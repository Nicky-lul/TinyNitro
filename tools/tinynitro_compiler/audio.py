from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
import audioop
import math
import wave

from formats import AudioExtra, AudioFormat

FLAG_LOOP = 1


@dataclass
class CompiledAudio:
    name: str
    data: bytes
    extra: AudioExtra


def _to_mono(raw: bytes, sample_width: int, channels: int) -> bytes:
    if channels == 1:
        return raw
    if channels == 2:
        return audioop.tomono(raw, sample_width, 0.5, 0.5)
    raise ValueError(f"Unsupported channel count: {channels}. Use mono or stereo WAV.")


def _resample(raw: bytes, sample_width: int, channels: int, src_rate: int, dst_rate: int) -> bytes:
    if src_rate == dst_rate:
        return raw
    converted, _state = audioop.ratecv(raw, sample_width, channels, src_rate, dst_rate, None)
    return converted


def _to_unsigned_pcm8(raw: bytes, sample_width: int) -> bytes:
    if sample_width == 1:
        return raw

    if sample_width == 2:
        pcm8_signed = audioop.lin2lin(raw, 2, 1)
        return audioop.bias(pcm8_signed, 1, 128)

    raise ValueError(f"Unsupported sample width: {sample_width}. Use 8-bit or 16-bit WAV.")


def compile_wav_pcm8(name: str, wav_path: Path, sample_rate: int, chunk_size: int, loop: bool) -> CompiledAudio:
    with wave.open(str(wav_path), "rb") as wav:
        channels = wav.getnchannels()
        sample_width = wav.getsampwidth()
        src_rate = wav.getframerate()
        frame_count = wav.getnframes()
        raw = wav.readframes(frame_count)

    raw = _to_mono(raw, sample_width, channels)
    raw = _resample(raw, sample_width, 1, src_rate, sample_rate)
    pcm8 = _to_unsigned_pcm8(raw, sample_width)

    total_samples = len(pcm8)
    chunk_count = math.ceil(len(pcm8) / chunk_size) if chunk_size > 0 else 1
    flags = FLAG_LOOP if loop else 0

    extra = AudioExtra(
        sample_rate=sample_rate,
        channels=1,
        audio_format=AudioFormat.PCM8,
        total_samples=total_samples,
        chunk_size=chunk_size,
        chunk_count=chunk_count,
        flags=flags,
    )

    return CompiledAudio(name=name, data=pcm8, extra=extra)
