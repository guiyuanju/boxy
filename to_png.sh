#!/bin/bash

for ppm in *.ppm; do
  name=`echo "$ppm" | cut -d'.' -f1`
  echo "$name"
  magick "$ppm" "${name}.png"
done
