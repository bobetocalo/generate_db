generate_db
===========

Generate a synthetic characters database using C++, [OpenCV](http://www.opencv.org), [Boost](http://www.boost.org) and [Freetype](http://www.freetype.org/).

For each character of interest (digit, capital letter, lower case or delimiter), we are capable of producing high-quality bitmap images using a list of true type fonts. Finally, we process each frame using random smooth, morphological, affine and anistropic filters to improve a selected OCR classifier learning. For example, these are synthetic characters extracted from an OCR-B true type font accepted as standard for spanish national identification documents.

![Synthetic characters generated from OCR-B true type font](https://sites.google.com/site/bobetocalo/home/synthetic_characters.png?attredirects=0)
