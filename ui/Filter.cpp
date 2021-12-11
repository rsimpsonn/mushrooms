#include "Filter.h"
#include "glm/ext.hpp"
#include <iostream>

Filter::Filter()
{

}

unsigned char RGBAToGray(const RGBA &pixel) {
    // TODO: Task 5
    return (0.299f * pixel.r) + (0.587f * pixel.g) + (0.114f * pixel.b);

}

inline unsigned char REAL2byte(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}

void Filter::grayscale(Canvas2D* canvas) {
        int width = canvas->width();

        RGBA* current_pixel = nullptr;

        /* Initialize the first pixel of the first row */
        RGBA* current_row = canvas->data();

        RGBA* data = canvas->data();
        size_t currentIndex = 0;

        for (int r = 0; r < canvas->height(); r++) {
            current_pixel = current_row;
            currentIndex = r * width;

            for (int c = 0; c < canvas->width(); c++) {
                // TODO: Task 4
                unsigned char g = RGBAToGray(*current_pixel);


                // TODO: Task 6
                current_pixel->r = g;
                current_pixel->b = g;
                current_pixel->g = g;


                /* Advance to the next pixel */
                current_pixel++;
                currentIndex++;
            }
            current_row += width;
        }
    }

void Filter::edgeDetect(Canvas2D* canvas, float sensitivity) {
    RGBA* data = canvas->data();
    int width = canvas->width();
    int height = canvas->height();

    RGBA result [width * height];


    // TODO: Task 10 Obtain kernel dimension
    std::vector<float> kernelX = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    std::vector<float> kernelY = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int kernelDim = 3;


    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            size_t centerIndex = r * width + c;

            // TODO: Task 11 Initialize color to accumulate convolution data
            float xAcc = 0;
            float yAcc = 0;


            // TODO: Task 12
            // Iterate over the kernel using the value from task 10
            // Obtain the value at current index of kernel
            // Find correct index in original image data
            // Accumulate the kernel applied to pixel value in color_acc
            for (int i = 0; i < kernelDim; i++) {
                for (int j = 0; j < kernelDim; j++) {
                    float kValX = kernelX[i * kernelDim + j];
                    float kValY = kernelY[i * kernelDim + j];

                    float diffI = i - int(kernelDim / 2);
                    float diffJ = j - int(kernelDim / 2);
                    int pRow = r + diffI;
                    int cRow = c + diffJ;

                    if (pRow < 0 || pRow > height - 1 || cRow < 0 || cRow > width - 1) {
                        continue;
                    }

                    RGBA p = data[pRow * height + cRow];

                    xAcc += (float(p.r) / 255.f) * kValX;
                    yAcc += (float(p.r) / 255.f) * kValY;

                }
            }

            // TODO: Task 13 Ignore outside bounds

            float comb = std::min(1.f, float(sqrt(pow(xAcc, 2) + pow(yAcc, 2)) * sensitivity));


            // TODO: Task 14 Update buffer with accumulated color
            RGBA acc_color = RGBA(REAL2byte(comb), REAL2byte(comb), REAL2byte(comb));
            result[centerIndex] = acc_color;
        }
    }

    // TODO: Task 15 Copy over buffer to canvas data
    memcpy(data, result, height * width * sizeof(RGBA));
    }

void Filter::blur(Canvas2D* canvas, int r) {
    std::vector<float> kernel;
    int height = canvas->height();
    int width = canvas->width();
    RGBA* data = canvas->data();
    RGBA result [width * height];
    int kernelDim = r * 2 + 1;
    float mid = r + 0.5;
    float full = sqrt(2 * pow(mid, 2));
    float total = 0;

    for (int i = 0; i < kernelDim; i++) {
        for (int j = 0; j < kernelDim; j++) {
            float centerI = i + 0.5;
            float centerJ = j + 0.5;

            float val = 1.f - sqrt(pow(centerI - mid, 2) + pow(centerJ - mid, 2)) / full;
            total += val;

            kernel.push_back(val);
        }
    }

    for (int i = 0; i < kernelDim; i++) {
        for (int j = 0; j < kernelDim; j++) {
            kernel[i * kernelDim + j] = kernel[i * kernelDim + j] / total;
        }
    }

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            size_t centerIndex = r * width + c;

            // TODO: Task 11 Initialize color to accumulate convolution data
            float red_acc = 0;
            float green_acc = 0;
            float blue_acc = 0;

            float k_acc = 0;


            // TODO: Task 12
            // Iterate over the kernel using the value from task 10
            // Obtain the value at current index of kernel
            // Find correct index in original image data
            // Accumulate the kernel applied to pixel value in color_acc
            for (int i = 0; i < kernelDim; i++) {
                for (int j = 0; j < kernelDim; j++) {
                    float kVal = kernel[i * kernelDim + j];

                    float diffI = i - int(kernelDim / 2);
                    float diffJ = j - int(kernelDim / 2);
                    int pRow = r + diffI;
                    int cRow = c + diffJ;

                    if (pRow < 0 || pRow > height - 1 || cRow < 0 || cRow > width - 1) {
                        continue;
                    }

                    RGBA p = data[pRow * height + cRow];
                    k_acc += kVal;

                    red_acc += (float(p.r) / 255.f) * kVal;
                    green_acc += (float(p.g) / 255.f) * kVal;
                    blue_acc += (float(p.b) / 255.f) * kVal;

                }
            }

            // TODO: Task 13 Ignore outside bounds


            // TODO: Task 14 Update buffer with accumulated color
            RGBA acc_color = RGBA(REAL2byte(red_acc / k_acc), REAL2byte(green_acc / k_acc), REAL2byte(blue_acc / k_acc));
            result[centerIndex] = acc_color;
        }
    }

    // TODO: Task 15 Copy over buffer to canvas data
    memcpy(data, result, height * width * sizeof(RGBA));

}

void Filter::scaleHorizontal(Canvas2D* canvas, float x) {
    int originalWidth = canvas->width();
    int newWidth = canvas->width() * x;
    int height = canvas->height();
    RGBA *data = canvas->data();
    RGBA result [newWidth * height];

    float a = float(newWidth) / originalWidth;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < newWidth; j++) {
            float backMapped = float(j) / a + (1.f - a)/(2*a);
            float r = a > 1 ? 1 : 1.f / a;
            int left = ceil(backMapped - r);
            int right = floor(backMapped + r);

            float sumR = 0;
            float sumG = 0;
            float sumB = 0;

            float weights = 0;

            for (int k = left; k <= right; k++) {
                if (k >= 0 && k < originalWidth) {
                    float weight = abs(k - backMapped) / r;
                    sumR += float(data[i * originalWidth + k].r / 255.f) * weight;
                    sumG += float(data[i * originalWidth + k].g / 255.f) * weight;
                    sumB += float(data[i * originalWidth + k].b / 255.f) * weight;
                    weights += weight;
                }
            }

            RGBA acc_color = RGBA(REAL2byte(sumR / weights), REAL2byte(sumG / weights), REAL2byte(sumB / weights));
            result[i * newWidth + j] = acc_color;
        }
    }


    // TODO: Task 15 Copy over buffer to canvas data
    canvas->resize(newWidth, height);
    memcpy(canvas->data(), result, height * newWidth * sizeof(RGBA));
}

void Filter::scaleVertical(Canvas2D* canvas, float y) {
    int originalHeight = canvas->height();
    int newHeight = canvas->height() * y;
    int width = canvas->width();
    RGBA *data = canvas->data();
    RGBA result [newHeight * width];

    float a = float(newHeight) / originalHeight;

    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < width; j++) {
            float backMapped = float(i) / a + (1.f - a)/(2*a);
            float r = a > 1 ? 1 : 1.f / a;
            int left = ceil(backMapped - r);
            int right = floor(backMapped + r);

            float sumR = 0;
            float sumG = 0;
            float sumB = 0;

            float weights = 0;

            for (int k = left; k <= right; k++) {
                if (k >= 0 && k < originalHeight) {
                    float weight = abs(k - backMapped) / r;
                    sumR += float(data[k * width + j].r / 255.f) * weight;
                    sumG += float(data[k * width + j].g / 255.f) * weight;
                    sumB += float(data[k * width + j].b / 255.f) * weight;
                    weights += weight;
                }
            }

            RGBA acc_color = RGBA(REAL2byte(sumR / weights), REAL2byte(sumG / weights), REAL2byte(sumB / weights));
            result[i * width + j] = acc_color;
        }
    }


    // TODO: Task 15 Copy over buffer to canvas data
    canvas->resize(width, newHeight);
    memcpy(canvas->data(), result, newHeight * width * sizeof(RGBA));
}



void Convolve2D(RGBA* data, int width, int height, const std::vector<float> &kernel) {
    // TODO: Task 9 Create buffer to store new image data
    RGBA result [width * height];


    // TODO: Task 10 Obtain kernel dimension
    int ks = kernel.size();
    int kernelDim = sqrt(ks);


    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            size_t centerIndex = r * width + c;

            // TODO: Task 11 Initialize color to accumulate convolution data
            float red_acc = 0;
            float green_acc = 0;
            float blue_acc = 0;


            // TODO: Task 12
            // Iterate over the kernel using the value from task 10
            // Obtain the value at current index of kernel
            // Find correct index in original image data
            // Accumulate the kernel applied to pixel value in color_acc
            for (int i = 0; i < kernelDim; i++) {
                for (int j = 0; j < kernelDim; j++) {
                    float kVal = kernel[i * kernelDim + j];

                    float diffI = i - int(kernelDim / 2);
                    float diffJ = j - int(kernelDim / 2);
                    int pRow = r + diffI;
                    int cRow = c + diffJ;

                    if (pRow < 0 || pRow > height - 1 || cRow < 0 || cRow > width - 1) {
                        continue;
                    }

                    RGBA p = data[pRow * height + cRow];

                    red_acc += (float(p.r) / 255.f) * kVal;
                    green_acc += (float(p.g) / 255.f) * kVal;
                    blue_acc += (float(p.b) / 255.f) * kVal;

                }
            }

            // TODO: Task 13 Ignore outside bounds


            // TODO: Task 14 Update buffer with accumulated color
            RGBA acc_color = RGBA(REAL2byte(red_acc), REAL2byte(green_acc), REAL2byte(blue_acc));
            result[centerIndex] = acc_color;
        }
    }

    // TODO: Task 15 Copy over buffer to canvas data
    memcpy(data, result, height * width * sizeof(RGBA));

}
