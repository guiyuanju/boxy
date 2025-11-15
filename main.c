#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define CANVAS_SIZE 5
#define WHITE 0xffffff
#define BLACK 0x000000
#define color_t int
#define R(color) (color & 0xff)
#define G(color) ((color >> 8) & 0xff)
#define B(color) ((color >> 16) & 0xff)

const color_t FACE_NORMAL[] = {
    0, 0, 0, 0, 0, // 1
    0, 1, 0, 1, 0, // 2
    0, 1, 0, 1, 0, // 3
    0, 0, 0, 0, 0, // 4
    0, 0, 0, 0, 0, // 5
};

const color_t FACE_NORMAL_UP[] = {
    0, 1, 0, 1, 0, // 1
    0, 1, 0, 1, 0, // 2
    0, 0, 0, 0, 0, // 3
    0, 0, 0, 0, 0, // 4
    0, 0, 0, 0, 0, // 5
};

const color_t FACE_NORMAL_DOWN[] = {
    0, 0, 0, 0, 0, // 1
    0, 0, 0, 0, 0, // 2
    0, 1, 0, 1, 0, // 3
    0, 1, 0, 1, 0, // 4
    0, 0, 0, 0, 0, // 5
};

const color_t FACE_NORMAL_LEFT[] = {
    0, 0, 0, 0, 0, // 1
    1, 0, 1, 0, 0, // 2
    1, 0, 1, 0, 0, // 3
    0, 0, 0, 0, 0, // 4
    0, 0, 0, 0, 0, // 5
};

const color_t FACE_NORMAL_RIGHT[] = {
    0, 0, 0, 0, 0, // 1
    0, 0, 1, 0, 1, // 2
    0, 0, 1, 0, 1, // 3
    0, 0, 0, 0, 0, // 4
    0, 0, 0, 0, 0, // 5
};

const color_t FACE_SUSPECT[] = {
    0, 0, 0, 0, 0, // 1
    0, 1, 0, 1, 0, // 2
    0, 0, 0, 0, 0, // 3
    0, 0, 0, 0, 0, // 4
    0, 0, 0, 0, 0, // 5
};

const color_t FACE_SHOCK[] = {
    0, 0, 0, 0, 0, // 1
    1, 1, 0, 1, 1, // 2
    1, 1, 0, 1, 1, // 3
    0, 0, 0, 0, 0, // 4
    0, 0, 0, 0, 0, // 5
};

const color_t FACE_SAD[] = {
    0, 0, 0, 0, 0, // 1
    0, 1, 0, 1, 0, // 2
    0, 1, 0, 1, 0, // 3
    0, 1, 0, 1, 0, // 4
    0, 0, 0, 0, 0, // 5
};

const color_t FACE_HAPPY[] = {
    0, 0, 0, 0, 0, // 1
    0, 1, 0, 1, 0, // 2
    1, 0, 1, 0, 1, // 3
    0, 0, 0, 0, 0, // 4
    0, 0, 0, 0, 0, // 5
};

const color_t FACE_SHY[] = {
    0, 0, 0, 0, 0, // 1
    0, 1, 0, 1, 0, // 2
    0, 0, 0, 0, 0, // 3
    1, 0, 0, 0, 1, // 4
    0, 0, 0, 0, 0, // 5
};

typedef struct {
  const char *name;
  const color_t *face;
} Face;

const Face FACES[] = {
    {.name = "normal", .face = FACE_NORMAL},
    {.name = "normal_up", .face = FACE_NORMAL_UP},
    {.name = "normal_down", .face = FACE_NORMAL_DOWN},
    {.name = "normal_left", .face = FACE_NORMAL_LEFT},
    {.name = "normal_right", .face = FACE_NORMAL_RIGHT},
    {.name = "happy", .face = FACE_HAPPY},
    {.name = "sad", .face = FACE_SAD},
    {.name = "shy", .face = FACE_SHY},
    {.name = "suspect", .face = FACE_SUSPECT},
};

void fillCell(char *buffer, int imgSize, float cellSize, int row, int col,
              color_t color) {
  char r = R(color);
  char g = G(color);
  char b = B(color);

  for (int i = row; i < row + cellSize; i++) {
    for (int j = col; j < col + cellSize; j++) {
      int idx = (imgSize * i + j) * 3;
      buffer[idx] = r;
      buffer[idx + 1] = g;
      buffer[idx + 2] = b;
    }
  }
}

void writePPM(const char *name, int width, int height, char *buffer,
              size_t bufferLen) {
  FILE *fp = fopen(name, "w");
  if (fp == NULL) {
    printf("failed to open file\n");
    exit(1);
  }

  fprintf(fp, "P6");
  fprintf(fp, " ");
  fprintf(fp, "%d", width); // width
  fprintf(fp, " ");
  fprintf(fp, "%d", height); // height
  fprintf(fp, " ");
  fprintf(fp, "%d", 255); // maximum color value, [1, 65535]
  fprintf(fp, "\n");

  fwrite(buffer, sizeof(char), bufferLen, fp);

  fclose(fp);
}

// draw to buffer from canvas with cellSize
void draw(char *buffer, int imgSize, const color_t *canvas, int canvasSize,
          int cellSize) {
  for (int i = 0; i < canvasSize; i++) {
    for (int j = 0; j < canvasSize; j++) {
      fillCell(buffer, imgSize, cellSize, i * cellSize, j * cellSize,
               canvas[canvasSize * i + j] == 0 ? BLACK : WHITE);
    }
  }
}

void printHelp() { printf("Usage: COMMAND <image size>"); }

int main(int argc, char **argv) {
  if (argc < 1) {
    printHelp();
    exit(1);
  }
  int imgSize = 200;
  if (argc > 1) {
    imgSize = atoi(argv[1]);
  }

  int cellSize = imgSize / CANVAS_SIZE;
  int bufferLen = sizeof(char) * 3 * imgSize * imgSize;
  char *buffer = malloc(bufferLen);
  char filename[32] = {0};

  for (int i = 0; i < sizeof(FACES) / sizeof(FACES[0]); i++) {
    snprintf(filename, sizeof(filename), "%s.ppm", FACES[i].name);
    draw(buffer, imgSize, FACES[i].face, CANVAS_SIZE, cellSize);
    writePPM(filename, imgSize, imgSize, buffer, bufferLen);
  }

  free(buffer);
}
