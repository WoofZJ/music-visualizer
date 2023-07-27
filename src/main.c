#include "raylib.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include "fft.h"

#define N 32768
#define BUFFER_SIZE 32768
#define BAR_SIZE 100

typedef struct {
    float left, right;
} Frame;

Frame frames_buffer[BUFFER_SIZE];
size_t frames_cnt;

float fit(float from, float e)
{
    float to = ((int)(from/e))*e;
    if (from - to > e/2) {
        to += e;
    }
    return to;
}

void callback(void* buffer, unsigned int frames)
{
    if (frames_cnt+frames <= BUFFER_SIZE) {
        memcpy(frames_buffer+frames_cnt, buffer, sizeof(Frame)*frames);
        frames_cnt += frames;
    } else if (frames < frames_cnt) {
        memmove(frames_buffer, frames_buffer+frames, sizeof(Frame)*(frames_cnt-frames));
        memcpy(frames_buffer+frames_cnt-frames, buffer, sizeof(Frame)*frames);
    } else {
        assert(0);
    }
}

int main()
{
    InitWindow(1600, 1000, "CAO - Music Visualizer");
    InitAudioDevice();

    Music music = LoadMusicStream("assets/music/SeeYouAgain.mp3");
    PlayMusicStream(music);
    AttachAudioStreamProcessor(music.stream, callback);

    char text[100];
    char label[BAR_SIZE+3][100];
    
    size_t n = N;
    float in[N];
    float complex out[N];
    float res[BAR_SIZE+3];
    float top_res[BAR_SIZE+3] = {0};
    float max, maxmax=0;

    float w = GetRenderWidth();
    float h = 400;

    float span = log10f(20000)-log10f(20);

    SetTargetFPS(30);
    for (size_t i = 20, j=0; i <= 20000; ++i) {
        int index = (int)((log10f(i)-log10f(20))/(span/BAR_SIZE));
        if (index >= j) {
            snprintf(label[index], 50, "%d", i);
            ++j;
        }
    }

    while (!WindowShouldClose()) {
        UpdateMusicStream(music);


        if (IsMusicStreamPlaying(music)) {
            max = 0;

            for (size_t i = 0; i < n; ++i) {
                in[i] = frames_buffer[i].left;
            }
            fft(in, 1, out, n);
            memset(res, 0, sizeof(res));
            for (size_t i = 20; i <= 20000; ++i) {
                int index = (int)((log10f(i)-log10f(20))/(span/BAR_SIZE));
                res[index] += cabsf(out[i]);
                if (res[index] > max) {
                    max = res[index];
                }
            }
            if (max > maxmax) {
                maxmax = max;
            }
            if (max > 500) {
                for (size_t i = 0; i < BAR_SIZE; ++i) {
                    res[i] = fit(res[i]/max, 0.05);
                    top_res[i] -= 0.015;
                    if (top_res[i] < res[i]) {
                        top_res[i] = res[i];
                    }
                }
            } else {
                for (size_t i = 0; i < BAR_SIZE; ++i) {
                    res[i] = fit(res[i]/maxmax, 0.05);
                    top_res[i] -= 0.015;
                    if (top_res[i] < res[i]) {
                        top_res[i] = res[i];
                    }
                }
            }

        }

        if (IsKeyPressed(KEY_SPACE)) {
            if (IsMusicStreamPlaying(music)) {
                PauseMusicStream(music);
            } else {
                ResumeMusicStream(music);
            }
        }

        float played = GetMusicTimePlayed(music);
        snprintf(text, 50, "Music has played for %3.2f s.", played);


        BeginDrawing();

        ClearBackground(WHITE);

        DrawText(text, 0, 0, 24, BLACK);
        if (!IsMusicStreamPlaying(music)) {
            DrawText(" - Paused", 350, 0, 24, RED);

        }
        
        for (size_t i = 0; i < frames_cnt; ++i) {
            float value = frames_buffer[i].left;
            if (value > 0) {
                DrawRectangle(i*w/frames_cnt, (h/2)*(1-value)+1, w/frames_cnt+1, (h/2)*value, GREEN);
                DrawRectangle(i*w/frames_cnt, h/2, w/frames_cnt+1, (h/2)*value, GREEN);
            } else {
                DrawRectangle(i*w/frames_cnt, (h/2)*(1+value)+1, w/frames_cnt+1, (h/2)*(-value), GREEN);
                DrawRectangle(i*w/frames_cnt, h/2, w/frames_cnt+1, (h/2)*(-value), GREEN);
            }
        }

        float ratio = max/maxmax;
        DrawText("Strength", w/2-60, 380, 30, BLACK);
        for (size_t i = 0; w/2 -i*20 >= w/2*(1-ratio); ++i) {
            DrawRectangle(w/2-20*i, 420, 15, 40, ORANGE);
            DrawRectangle(w/2+20*i, 420, 15, 40, ORANGE);
        }
        
        for (size_t i = 0; i < BAR_SIZE; ++i) {
            float value = res[i];
            float top_value = top_res[i];
            DrawRectangle(i*w/BAR_SIZE, 600+h/1.5*(1-value), w/BAR_SIZE-6, h/1.5*value, BLUE);
            DrawRectangle(i*w/BAR_SIZE, 600+h/1.5*(1-top_value)-6, w/BAR_SIZE-6, 6, PURPLE);
        }
        for (size_t i = 0; i < BAR_SIZE; i += 5) {
            DrawText(label[i], i*w/BAR_SIZE, 610+h/1.5, 24, BLACK);
        }
        
        EndDrawing();
    }
    return 0;
}