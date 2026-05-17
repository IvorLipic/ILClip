# ILClip

A waveform clipper / distortion effect plugin (VST3, AU, Standalone) built with **JUCE 8**.

Inspired by [GClip by GVST](https://gvst.uk/Downloads/GClip). Created as an audioprogramming project at **FER** (Faculty of Electrical Engineering and Computing, University of Zagreb).

## Features

- **3 clipping algorithms** selectable at runtime:
  - **Hard Clip** — samples exceeding the threshold are clamped: `clamp(x, -T, +T)`
  - **Soft Clip (Hyperbolic Tangent)** — smooth saturation via `tanh(x/T) * T`
  - **S-Shaped Sigmoid** — sigmoid waveshaping: `(x / (1 + |x|)) * T`
- **Threshold control** — adjustable from 0.0 to 1.0 (default 0.8)
- **Real-time waveform display** — 60 FPS waveform view with threshold guidelines
- **State persistence** — parameter values saved/restored via `AudioProcessorValueTreeState`

## Building from Source

### Requirements

- [JUCE 8](https://juce.com/) — set the `JUCE_8` path in the `.jucer` file (or adjust module paths in the build projects)
- Visual Studio 2022 (Windows) or Xcode (macOS)
- Windows SDK 10.0 (for VST3 builds)

### Steps

1. Open `ILClip.jucer` in **Projucer** to regenerate build projects and `JuceLibraryCode/`.
2. Open `Builds/VisualStudio2022/ILClip.sln` in VS 2022.
3. Select **x64** configuration (Debug or Release) and build.

### Output

| Configuration | Artifact |
|---|---|
| VST3 | `ILClip.vst3\Contents\x86_64-win\ILClip.vst3` |
| Standalone | `ILClip.exe` |
| AU | Builds on macOS only |


## Controls

- **Threshold** — `AudioParameterFloat` (0.0–1.0, step 0.01, default 0.8)
- **Algorithm** — `AudioParameterChoice`:
  - `0` — Hard Clip
  - `1` — Soft Clip (Tanh)
  - `2` — S-Shaped Sigmoid

## Credits

- **Inspiration:** [GClip](https://gvst.uk/Downloads/GClip) by GVST
- **Framework:** [JUCE](https://juce.com/)
