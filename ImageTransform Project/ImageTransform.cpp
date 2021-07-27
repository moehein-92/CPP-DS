#include <iostream>
#include <cmath>
#include <cstdlib>

#include "uiuc/PNG.h"
#include "uiuc/HSLAPixel.h"
#include "ImageTransform.h"

/* ******************
(Begin multi-line comment...)

Write your name and email address in the comment space here:

Name: Moe Hein Aung
Email: moeheinag@gmail.com

(...end multi-line comment.)
******************** */

using uiuc::PNG;
using uiuc::HSLAPixel;

PNG grayscale(PNG image) {
    for (unsigned x = 0; x < image.width(); x++) {
      for (unsigned y = 0; y < image.height(); y++) {
        HSLAPixel & pixel = image.getPixel(x, y);
      pixel.s = 0;}}
  return image;}

PNG createSpotlight(PNG image, int centerX, int centerY) {
      for (unsigned x = 0; x < image.width(); x++) {
        for (unsigned y = 0; y < image.height(); y++) {
          HSLAPixel & pixel = image.getPixel(x, y);

          unsigned int w = x>centerX ? x-centerX : centerX-x;
          unsigned int t = y>centerY ? y-centerY : centerY-y;
          double sum = sqrt((w*w) + (t*t));
          if (sum >= 0 && sum <= 160){
            pixel.l *= (1-(0.005*sum));
          }
          else {pixel.l *= 0.2;}}}
          return image;}

PNG illinify(PNG image) {
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);
      //double diff1 = pixel.h - 11;
      //double diff2 = 216 - pixel.h;
      if (pixel.h <= 114 || pixel.h >= 294){
        pixel.h = 11;}
      else {pixel.h = 216;}}}
      //Orange (11) Blue (216)
      return image;}

PNG watermark(PNG firstImage, PNG secondImage) {
  for (unsigned x = 0; x < secondImage.width(); x++) {
    for (unsigned y = 0; y < secondImage.height(); y++) {
      HSLAPixel & pixel2 = secondImage.getPixel(x, y);
      HSLAPixel & pixel1 = firstImage.getPixel(x,y);
      if (pixel2.l == 1 && (pixel1.l+0.2) <= 1) {
        pixel1.l += 0.2;}}}
  return firstImage;}
