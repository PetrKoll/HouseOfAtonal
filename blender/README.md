# Blender Model

## Current asset

`HouseOfAtonal_blockout_v01.blend` is a parametric architectural blockout based
on the supplied presentation board.

The source board does not contain readable construction dimensions. The current
model therefore establishes spatial intent and approximate proportions rather
than construction accuracy.

## Regenerate

Open the repository root in a terminal and run:

```bash
/Applications/Blender.app/Contents/MacOS/Blender \
  --background \
  --python blender/scripts/build_house_blockout.py
```

The script recreates the `.blend` file and preview render. Change the parameters
at the top of the script when verified dimensions become available.

## Working assumptions

- Overall footprint: 12 × 14 m
- Three above-ground floors
- Floor-to-floor height: 3.2 m
- Courtyard opening: 5.2 × 7 m
- Reinforced-concrete structural and façade language
- Glazed courtyard elevations with bronze aluminium framing
- Street-side entry and garage

## Model organization

- `A_STRUCTURE`: slabs
- `B_FACADE`: external envelope and parapets
- `C_GLAZING`: courtyard glazing, frames and windows
- `D_SITE`: site and courtyard placeholders
- `Z_PRESENTATION`: preview camera and lights

This version is intended for proportion review. Interior partitions, stairs,
doors, detailed façade joints, furniture and final Unreal-ready cleanup belong
to later versions.

