# digital-image-processing

Here are the programs that I wrote during the Digital Image Processing class in 2008, as well as some new experiments.

1. Grayscale to black-and-white
    1. [Thresholding](#thresholding-lab1_1c)
    2. [Floyd-Steinberg dithering](#floyd-steinberg-dithering-lab1_2c)
    3. [Jarvis-Judice-Ninke dithering](#jarvis-judice-ninke-dithering-lab1_3c)
    4. [Atkinson dithering](#atkinson-dithering-lab1_4c)
    5. [Ordered dithering](#ordered-dithering-lab1_5c)
    6. [Random dithering](#random-dithering-lab1_6c)
    7. [Clustered dithering](#clustered-dithering-lab1_7c)
2. Image scaling
    1. [Nearest-neighbor interpolation](#nearest-neighbor-interpolation-lab2_1c)
    2. [Bilinear interpolation](#bilinear-interpolation-lab2_2c)
3. [Alpha compositing](#alpha-compositing-lab3c)
4. Intensity transformations
    1. [Gamma correction](#gamma-correction-lab4_1c)
    2. [Linear contrast stretching](#linear-contrast-stretching-lab4_2c)
5. [Convolution](#convolution-lab5c)
6. [Median filter](#median-filter-lab6c)
7. [Segmentation](#segmentation-lab7c)

---
## Thresholding ([lab1_1.c](lab1_1/lab1_1.c))
| Input | Output (threshold = 100) |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab1_1/Lena_out_100.png) |

---
## Floyd-Steinberg dithering ([lab1_2.c](lab1_2/lab1_2.c))
| Input | Output |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab1_2/Lena_out.png) |

---
## Jarvis-Judice-Ninke dithering ([lab1_3.c](lab1_3/lab1_3.c))
| Input | Output |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab1_3/Lena_out.png) |

---
## Atkinson dithering ([lab1_4.c](lab1_4/lab1_4.c))
| Input | Output |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab1_4/Lena_out.png) |

---
## Ordered dithering ([lab1_5.c](lab1_5/lab1_5.c))
| Input | Output (4x4 Bayer matrix) |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab1_5/Lena_out_4.png) |

---
## Random dithering ([lab1_6.c](lab1_6/lab1_6.c))
| Input | Output (threshold = 100, noise = 40) |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab1_6/Lena_out_100_40.png) |

---
## Clustered dithering ([lab1_7.c](lab1_7/lab1_7.c))
| Input | Output |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab1_7/Lena_out.png) |

---
## Nearest-neighbor interpolation ([lab2_1.c](lab2_1/lab2_1.c))
| Input | Output (N = 5, cropped) |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab2_1/Lena_out_5x_crop.png) |

---
## Bilinear interpolation ([lab2_2.c](lab2_2/lab2_2.c))
| Input | Output (N = 5, cropped) |
| ----- | ------ |
| ![Input](images/Lena.png) | ![Output](lab2_2/Lena_out_5x_crop.png) |

---
## Alpha compositing ([lab3.c](lab3/lab3.c))
| Input | Output |
| ----- | ------ |
| <ul><li>[Goldhill.png](images/Goldhill.png) as top</li><li>[Lena.png](images/Lena.png) as bottom</li><li>[Peppers.png](images/Peppers.png) as alpha channel</li></ul> | ![Output](lab3/Goldhill_Lena_Peppers_out.png) |

---
## Gamma correction ([lab4_1.c](lab4_1/lab4_1.c))
| Input | Output (c = 1.0, gamma = 0.6) |
| ----- | ------ |
| ![Input](images/House.png) | ![Output](lab4_1/House_out_1.0_0.6.png) |
| ![Input](lab4_1/House_in_hist.png) | ![Output](lab4_1/House_out_1.0_0.6_hist.png) |

| Input | Output (c = 1.5, gamma = 1.5) |
| ----- | ------ |
| ![Input](images/House.png) | ![Output](lab4_1/House_out_1.5_1.5.png) |
| ![Input](lab4_1/House_in_hist.png) | ![Output](lab4_1/House_out_1.5_1.5_hist.png) |

---
## Linear contrast stretching ([lab4_2.c](lab4_2/lab4_2.c))
| Input | Output (delta = 1000) |
| ----- | ------ |
| ![Input](images/Auto.png) | ![Output](lab4_2/Auto_out_1000.png) |
| ![Input](lab4_2/Auto_in_hist.png) | ![Output](lab4_2/Auto_out_1000_hist.png) |

---
## Convolution ([lab5.c](lab5/lab5.c))
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
## Median filter ([lab6.c](lab6/lab6.c))
| Input | Output ([3x3 plus](apertures/Plus_3.txt) aperture) |
| ----- | ------ |
| ![Input](images/Goldhill_sap.png) | ![Output](lab6/Goldhill_sap_out_Plus_3.png) |

---
## Segmentation ([lab7.c](lab7/lab7.c))
| Input | Output (threshold = 128) |
| ----- | ------ |
| ![Input](images/Numbers.png) | ![Output](lab7/Numbers_out_128.png)<br>12 segments |

| Input (cropped) | Output (threshold = 80, cropped) |
| ----- | ------ |
| ![Input](lab7/Stars_in_crop.png) | ![Output](lab7/Stars_out_80_crop.png)<br>397 segments |

