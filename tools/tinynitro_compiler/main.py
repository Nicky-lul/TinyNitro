from __future__ import annotations

import argparse
import json
from pathlib import Path

from audio import compile_wav_pcm8
from formats import AssetType, AudioFormat
from packer import PackedAsset, write_pack


def load_project(path: Path) -> dict:
    with path.open("r", encoding="utf-8") as file:
        return json.load(file)


def compile_project(project_path: Path, out_path: Path) -> None:
    project = load_project(project_path)
    base_dir = project_path.parent
    assets: list[PackedAsset] = []

    for item in project.get("audio", []):
        name = item["name"]
        src = base_dir / item["file"]
        fmt = item.get("format", "pcm8").lower()
        sample_rate = int(item.get("sample_rate", 22050))
        stream = bool(item.get("stream", False))
        chunk_size = int(item.get("chunk_size", 32768 if stream else 0))
        loop = bool(item.get("loop", False))

        if fmt != "pcm8":
            raise ValueError(f"Unsupported audio format for now: {fmt}")

        compiled = compile_wav_pcm8(
            name=name,
            wav_path=src,
            sample_rate=sample_rate,
            chunk_size=chunk_size,
            loop=loop,
        )

        assets.append(
            PackedAsset(
                name=name,
                asset_type=AssetType.AUDIO,
                asset_format=AudioFormat.PCM8,
                data=compiled.data,
                extra=compiled.extra.pack(),
            )
        )

        print(f"[audio] {name}: {src} -> pcm8 {sample_rate}Hz, {len(compiled.data)} bytes")

    write_pack(out_path, assets)
    print(f"[pack] wrote {out_path} with {len(assets)} assets")


def main() -> int:
    parser = argparse.ArgumentParser(description="TinyNitro asset compiler")
    parser.add_argument("project", type=Path, help="Path to .tnproj JSON file")
    parser.add_argument("-o", "--output", type=Path, required=True, help="Output .tnpak path")
    args = parser.parse_args()

    compile_project(args.project, args.output)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
