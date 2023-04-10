#include <time.h>
#include "odu_33.h"
#include "supportLib.h"
#include "pbPlots.h"

#pragma warning (disable: 4996)


int Input(double *a, double *b, double *c, double *d, double *h, double *e) {
    FILE *fin;
    if ((fin = fopen("C:\\programs\\praktikum33\\fin.txt", "r")) == NULL) {
        fprintf(stderr, "Failed to open file");
        exit(-1);
    }

    fseek(fin, 0, SEEK_END);
    long pos = ftell(fin);
    if (pos <= 0) {
        fprintf(stderr, "File is empty");
        fclose(fin);
        exit(-2);
    }
    fseek(fin, 0, SEEK_SET);
//    char *end,*pointer_str, str[10];
//    gets(str);
    fscanf(fin, "%lf, %lf, %lf, %lf, %lf, %lf", a, b, c, d, h, e);
    fclose(fin);
//    fscanf(fin,"%s",str);
////    end=str;
//    *a = strtod(str, &end);
//    fscanf(fin,"%s",str);
//    *b = strtod(str, &end);
//    fscanf(fin,"%s",str);
//    *c = strtod(str, &end);
//    fscanf(fin,"%s",str);
//    *d = strtod(str, &end);
//    fscanf(fin,"%s",str);
//    *h = strtod(str, &end);
//    fscanf(fin,"%s",str);
//    *e = strtod(str, &end);

//
////    printf("%lf, %lf, %lf, %lf, %lf, %lf ",*a, *b, *c, *d, *h, *e);
//    if(end[0]==',')
//        printf("%s",end);
//    exit(0);

    if ((*h) <= 0 || (*e) < 0 || (*a) > (*b)  || (*b-*a)<(*h)) {
        fprintf(stderr, "Invalid values entered");
        exit(-3);
    }
    return 0;
}

void answer(double **y, double **x, double a, double h, int n) {
    double x_ = a;
    for (int i = 0; i < n; i++) {
        (*y)[i] = exactAnswer(x_);
        (*x)[i] = x_;
        x_ += h;
    }
}

double Different(double *y, const double *y_, int n) {
    double accuracy = 0;
    for (int i = 0; i < n; i++) {
        accuracy += (y[i] - y_[i]) * (y[i] - y_[i]);
        y[i]=y_[i];
    }
    return sqrt(accuracy);
}

int drawGraph(double *abscissa, double *exactAnswer, double *myAnswer, int n) {
    _Bool success;
    StringReference *errorMessage;

    ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
    series->xs = abscissa;
    series->xsLength = n;
    series->ys = exactAnswer;
    series->ysLength = n;
    series->linearInterpolation = true;
    series->lineType = L"solid";
    series->lineTypeLength = wcslen(series->lineType);
    series->lineThickness = 10;
    series->color = CreateRGBColor(0, 0, 1);

    ScatterPlotSeries *series1 = GetDefaultScatterPlotSeriesSettings();
    series1->xs = abscissa;
    series1->xsLength = n;
    series1->ys = myAnswer;
    series1->ysLength = n;
    series1->linearInterpolation = true;
    series1->lineType = L"solid";
    series1->lineTypeLength = wcslen(series1->lineType);
    series1->lineThickness = 3;
    series1->color = CreateRGBColor(1, 0, 0);

    ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
    settings->width = 1920;
    settings->height = 1080;
    settings->autoBoundaries = true;
    settings->autoPadding = true;
    settings->title = L"Func";
    settings->titleLength = wcslen(settings->title);
    settings->xLabel = L"X axis";
    settings->xLabelLength = wcslen(settings->xLabel);
    settings->yLabel = L"Y axis";
    settings->yLabelLength = wcslen(settings->yLabel);
    ScatterPlotSeries *s[] = {series, series1};
    settings->scatterPlotSeries = s;
    settings->scatterPlotSeriesLength = 2;

    RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
    errorMessage = (StringReference *) malloc(sizeof(StringReference));
    success = DrawScatterPlotFromSettings(canvasReference, settings, errorMessage);

    if (success) {
        size_t length;
        double *pngdata = ConvertToPNG(&length, canvasReference->image);
        WriteToFile(pngdata, length, "graph.png");
        DeleteImage(canvasReference->image);
    } else {
        fprintf(stderr, "Error: ");
        for (int i = 0; i < errorMessage->stringLength; i++) {
            fprintf(stderr, "%c", errorMessage->string[i]);
        }
        fprintf(stderr, "\n");
    }

    return success ? 0 : 1;
}

int OutPut(int n, double *y) {
    FILE *fout;
    fout = fopen("C:\\programs\\praktikum33\\fout.txt", "w");
    fprintf(fout, "%d\n", n);
    for (int i = 0; i < n; i++)
        fprintf(fout, "%lf ", y[i]);
    fclose(fout);
    return 0;
}

int main() {
    double a, b, c, d, h, e;
    Input(&a, &b, &c, &d, &h, &e);

    double accuracy = e + 1;
    int n = (int) round((b - a) / h) + 1;

    double *exact_y = (double *) malloc(n * sizeof(double));
    double *abscissa = (double *) malloc(n * sizeof(double));
    double *y = (double *) malloc(n * sizeof(double));
    double *y_ = (double *) malloc(n * sizeof(double));
    y[0] = c;
    y_[0]=c;

    answer(&exact_y, &abscissa, a, h, n); //точное решение
    Newton(y,a,b,d,h,e,n);

    while (accuracy > e) {
        h/=2;
        Newton(y_, a, b, d, h, e, n);// Метод Ньютона и разница с помощью МНК
        accuracy= Different(y,y_,n);
    }

    drawGraph(abscissa, exact_y, y, n);
    OutPut(n,y);
    free(abscissa);
    free(exact_y);
    free(y);
    free(y_);
    return 0;

}