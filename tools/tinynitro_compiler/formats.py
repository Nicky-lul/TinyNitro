from __future__ import annotations

from dataclasses import dataclass
from enum import IntEnum
import struct

MAGIC = b"TNPK"
VERSION = 1


class AssetType(IntEnum):
    AUDIO = 1
    IMAGE = 2
    MAP = 3


class AudioFormat(IntEnum):
    PCM8 = 1
    ADPCM = 2  # reserved for later


HEADER_STRUCT = struct.Struct("<4sHHII")
ASSET_STRUCT = struct.Struct("<32sHHIII")
AUDIO_EXTRA_STRUCT = struct.Struct("<IHHIIII")
# sample_rate, channels, format, total_samples, chunk_size, chunk_count, flags


@dataclass
class AssetEntry:
    name: str
    asset_type: int
    asset_format: int
    offset: int
    size: int
    extra_offset: int

    def pack(self) -> bytes:
        encoded_name = self.name.encode("ascii", errors="ignore")[:31]
        encoded_name += b"\0" * (32 - len(encoded_name))
        return ASSET_STRUCT.pack(
            encoded_name,
            int(self.asset_type),
            int(self.asset_format),
            int(self.offset),
            int(self.size),
            int(self.extra_offset),
        )


@dataclass
class AudioExtra:
    sample_rate: int
    channels: int
    audio_format: int
    total_samples: int
    chunk_size: int
    chunk_count: int
    flags: int = 0

    def pack(self) -> bytes:
        return AUDIO_EXTRA_STRUCT.pack(
            self.sample_rate,
            self.channels,
            int(self.audio_format),
            self.total_samples,
            self.chunk_size,
            self.chunk_count,
            self.flags,
        )
