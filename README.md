# digital-image-processing

Here are the programs that I wrote during the Digital Image Processing class in 2008, as well as some new experiments.

1. Grayscale to black-and-white
    1. [Thresholding](#thresholding)
    2. [Bit-plane slicing](#bit-plane-slicing)
    3. [Random dithering](#random-dithering)
    4. Ordered dithering
        1. [Bayer dithering](#bayer-dithering)
        2. [Clustered dithering](#clustered-dithering)
    5. Error-diffusion dithering
        1. [Floyd-Steinberg dithering](#floyd-steinberg-dithering)
        2. [Jarvis-Judice-Ninke dithering](#jarvis-judice-ninke-dithering)
        3. [Atkinson dithering](#atkinson-dithering)
2. Image scaling
    1. [Nearest-neighbor interpolation](#nearest-neighbor-interpolation)
    2. [Bilinear interpolation](#bilinear-interpolation)
3. [Alpha compositing](#alpha-compositing)
4. Intensity transformations
    1. [Gamma correction](#gamma-correction)
    2. [Linear contrast stretching](#linear-contrast-stretching)
    3. Histogram equalization
        1. [Histogram equalization](#histogram-equalization)
        2. [Adaptive histogram equalization](#adaptive-histogram-equalization)
5. [Convolution](#convolution)
6. [Median filter](#median-filter)
7. [Segmentation](#segmentation)

---
## [Thresholding](lab1_threshold/lab1_threshold.c)
| Input | Output (threshold = 100) |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab1_threshold/Lena_out_100.png) |

---
## [Bit-plane slicing](lab1_bitplane/lab1_bitplane.c)
| Output (bit 6) | Output (bit 5) |
| ----- | ------ |
| ![Output](lab1_bitplane/Lena_out_6.png) | ![Output](lab1_bitplane/Lena_out_5.png) |

| Output (bit 4) | Output (bit 3) |
| ----- | ------ |
| ![Output](lab1_bitplane/Lena_out_4.png) | ![Output](lab1_bitplane/Lena_out_3.png) |

---
## [Random dithering](lab1_random/lab1_random.c)
| Input | Output (threshold = 100, noise = 40) |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab1_random/Lena_out_100_40.png) |

---
## [Bayer dithering](lab1_bayer/lab1_bayer.c)
| Input | Output (4x4 Bayer matrix) |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab1_bayer/Lena_out_4.png) |

---
## [Clustered dithering](lab1_cluster/lab1_cluster.c)
| Input | Output |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab1_cluster/Lena_out.png) |

---
## [Floyd-Steinberg dithering](lab1_floyd/lab1_floyd.c)
| Input | Output |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab1_floyd/Lena_out.png) |

---
## [Jarvis-Judice-Ninke dithering](lab1_jarvis/lab1_jarvis.c)
| Input | Output |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab1_jarvis/Lena_out.png) |

---
## [Atkinson dithering](lab1_atkinson/lab1_atkinson.c)
| Input | Output |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab1_atkinson/Lena_out.png) |

---
## [Nearest-neighbor interpolation](lab2_nearest/lab2_nearest.c)
| Input | Output (N = 5, cropped) |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab2_nearest/Lena_out_5x_crop.png) |

---
## [Bilinear interpolation](lab2_bilinear/lab2_bilinear.c)
| Input | Output (N = 5, cropped) |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab2_bilinear/Lena_out_5x_crop.png) |

---
## [Alpha compositing](lab3/lab3.c)
| Input | Output |
| ----- | ------ |
| <ul><li>[Goldhill.png](images/Goldhill.png) as top</li><li>[Lena.png](images/Lena.png) as bottom</li><li>[Peppers.png](images/Peppers.png) as alpha channel</li></ul> | ![Output](lab3/Goldhill_Lena_Peppers_out.png) |

---
## [Gamma correction](lab4_gamma/lab4_gamma.c)
| Input | Output (c = 1.0, gamma = 0.6) |
| ----- | ------ |
| ![Input](images/House.png) | ![Output](lab4_gamma/House_out_1.0_0.6.png) |
| ![Input](lab4_gamma/House_in_hist.png) | ![Output](lab4_gamma/House_out_1.0_0.6_hist.png) |

| Input | Output (c = 1.5, gamma = 1.5) |
| ----- | ------ |
| ![Input](images/House.png) | ![Output](lab4_gamma/House_out_1.5_1.5.png) |
| ![Input](lab4_gamma/House_in_hist.png) | ![Output](lab4_gamma/House_out_1.5_1.5_hist.png) |

---
## [Linear contrast stretching](lab4_contrast/lab4_contrast.c)
| Input | Output (delta = 1000) |
| ----- | ------ |
| ![Input](images/Auto.png) | ![Output](lab4_contrast/Auto_out_1000.png) |
| ![Input](lab4_contrast/Auto_in_hist.png) | ![Output](lab4_contrast/Auto_out_1000_hist.png) |

---
## [Histogram equalization](lab4_equalize/lab4_equalize.c)
| Input | Output |
| ----- | ------ |
| ![Input](images/Goldhill.png) | ![Output](lab4_equalize/Goldhill_out.png) |
| ![Input](lab4_equalize/Goldhill_in_hist.png) | ![Output](lab4_equalize/Goldhill_out_hist.png) |

---
## [Adaptive histogram equalization](lab4_ahe/lab4_ahe.c)
| Input | Output (block size = 100) |
| ----- | ------ |
| ![Input](images/Goldhill.png) | ![Output](lab4_ahe/Goldhill_out_100.png) |
| ![Input](lab4_ahe/Goldhill_in_hist.png) | ![Output](lab4_ahe/Goldhill_out_100_hist.png) |

---
## [Convolution](lab5/lab5.c)
| Input | Output ([11x11 blur](kernels/Blur_11.txt) kernel) |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab5/Lena_out_Blur_11.png) |

| Input | Output ([Sharp](kernels/Sharp.txt) kernel) |
| ----- | ------ |
| ![Input](images/Goldhill.png) | ![Output](lab5/Goldhill_out_Sharp.png) |

| Input | Output ([Vertical Sobel](kernels/Sobel_v.txt) kernel) |
| ----- | ------ |
| ![Input](images/Peppers.png) | ![Output](lab5/Peppers_out_Sobel_v.png) |

---
## [Median filter](lab6/lab6.c)
| Input | Output ([3x3 plus](apertures/Plus_3.txt) aperture) |
| ----- | ------ |
| ![Input](images/Goldhill_sap.png) | ![Output](lab6/Goldhill_sap_out_Plus_3.png) |

---
## [Segmentation](lab7/lab7.c)
| Input | Output (threshold = 128) |
| ----- | ------ |
| ![Input](images/Numbers.png) | ![Output](lab7/Numbers_out_128.png)<br>12 segments |

| Input (cropped) | Output (threshold = 80, cropped) |
| ----- | ------ |
| ![Input](lab7/Stars_in_crop.png) | ![Output](lab7/Stars_out_80_crop.png)<br>397 segments |

