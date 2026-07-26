# Source Control Setup

Author: [FORGE]

## First setup on each computer

Git LFS must be installed before working with the repository's binary assets.

```text
git lfs install
git pull
git lfs pull
```

Use `git lfs env` to diagnose a machine that receives pointer text instead of
the actual binary files.

## LFS policy

`.gitattributes` is the source of truth. LFS currently tracks:

- Unreal packages: `.uasset`, `.umap`
- Blender and interchange sources: `.blend`, `.fbx`, `.glb`, `.abc`
- high-resolution source imagery: `.exr`, `.hdr`, `.psd`, `.tga`, `.tif`,
  `.tiff`
- production audio and video: `.wav`, `.flac`, `.mp4`, `.mov`

Ordinary project documentation, code, configuration and lightweight reference
images remain normal Git files.

## Safety

- Do not rewrite shared Git history to migrate old assets.
- Add any new large binary format to `.gitattributes` before committing files
  of that type.
- Confirm `git lfs status` before pushing a commit containing binary assets.
- A fresh clone on another computer must have Git LFS installed before Unreal
  or Blender files are opened.
