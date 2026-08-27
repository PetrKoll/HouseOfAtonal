import fs from "node:fs/promises";
import path from "node:path";
import { createRequire } from "node:module";

const require = createRequire(import.meta.url);
const sharp = require("sharp");

const icons = [
  "map-pin", "cloud-sun", "clock-3",
  "trees", "armchair", "bed-double",
  "sun", "cloud", "cloud-rain",
  "sunrise", "moon",
];

const output = path.resolve("res/lucide");
await fs.mkdir(output, { recursive: true });

for (const name of icons) {
  const url = `https://raw.githubusercontent.com/lucide-icons/lucide/main/icons/${name}.svg`;
  const response = await fetch(url);
  if (!response.ok) throw new Error(`${response.status} ${url}`);
  const svg = (await response.text())
    .replace(/stroke="currentColor"/g, 'stroke="#111111"')
    .replace(/stroke-width="2"/g, 'stroke-width="3.2"');
  await fs.writeFile(path.join(output, `${name}.svg`), svg);
  await sharp(Buffer.from(svg))
    .resize(256, 256)
    .png({ compressionLevel: 9 })
    .toFile(path.join(output, `${name}.png`));
}

const licenseResponse = await fetch(
  "https://raw.githubusercontent.com/lucide-icons/lucide/main/LICENSE",
);
if (!licenseResponse.ok) throw new Error(`License download failed: ${licenseResponse.status}`);
await fs.writeFile(path.join(output, "LICENSE.txt"), await licenseResponse.text());
