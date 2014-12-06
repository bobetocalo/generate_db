generate_db
===========

Generate a synthetic characters database using C++, [OpenCV](opencv.org), [Boost](www.boost.org) and [Freetype](http://www.freetype.org/).

For each digit, capital letter, lower case or delimiter of interest, we are capable of producing high-quality bitmap images using a list of true type fonts. Finally, we process each frame using random smooth, morphological, affine and anistropic filters to improve the OCR classifier quality. These are example synthetic characters extracted from an OCR-B true type font accepted as standard for spanish national identification documents.

![Synthetic characters generated from OCR-B true type font](https://sites.google.com/site/bobetocalo/home/synthetic_characters.png?attredirects=0)
