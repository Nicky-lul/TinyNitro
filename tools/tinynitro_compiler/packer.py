from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path

from formats import MAGIC, VERSION, HEADER_STRUCT, AssetEntry


@dataclass
class PackedAsset:
    name: str
    asset_type: int
    asset_format: int
    data: bytes
    extra: bytes


def _align(value: int, alignment: int = 4) -> int:
    return (value + alignment - 1) & ~(alignment - 1)


def _pad_to(blob: bytearray, offset: int) -> None:
    while len(blob) < offset:
        blob.append(0)


def write_pack(out_path: Path, assets: list[PackedAsset]) -> None:
    header_size = HEADER_STRUCT.size
    table_size = len(assets) * 48
    table_offset = header_size
    extra_offset = _align(header_size + table_size, 4)

    blob = bytearray()
    blob.extend(b"\0" * header_size)
    blob.extend(b"\0" * table_size)

    entries: list[AssetEntry] = []
    _pad_to(blob, extra_offset)

    extra_offsets: list[int] = []
    for asset in assets:
        if asset.extra:
            asset_extra_offset = _align(len(blob), 4)
            _pad_to(blob, asset_extra_offset)
            extra_offsets.append(asset_extra_offset)
            blob.extend(asset.extra)
        else:
            extra_offsets.append(0)

    data_offset = _align(len(blob), 4)
    _pad_to(blob, data_offset)

    for asset, asset_extra_offset in zip(assets, extra_offsets):
        asset_data_offset = _align(len(blob), 4)
        _pad_to(blob, asset_data_offset)
        blob.extend(asset.data)

        entries.append(
            AssetEntry(
                name=asset.name,
                asset_type=asset.asset_type,
                asset_format=asset.asset_format,
                offset=asset_data_offset,
                size=len(asset.data),
                extra_offset=asset_extra_offset,
            )
        )

    blob[0:header_size] = HEADER_STRUCT.pack(
        MAGIC,
        VERSION,
        len(assets),
        table_offset,
        data_offset,
    )

    cursor = table_offset
    for entry in entries:
        packed = entry.pack()
        blob[cursor:cursor + len(packed)] = packed
        cursor += len(packed)

    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_bytes(bytes(blob))
