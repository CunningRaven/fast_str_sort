#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* represents a region that we should sort. */
struct ss_frame {
    char **stra; /* the start point of this region */
    int cnt;     /* elements count */
    int offset;  /* which char of every string we should compare */
};

/* a stack of ss_frame */
struct ss_stack {
    struct ss_frame *frma; /* pointer to  the ss_frame array */
    int frms;  /* how many frames in the stack */
};

static inline int ss_stack_empty(struct ss_stack *stackp)
{
    return stackp->frms == 0;
}

static inline void ss_stack_push(struct ss_stack *stackp, 
        char **stra, int cnt, int offset)
{
    stackp->frma[stackp->frms].stra = stra;
    stackp->frma[stackp->frms].cnt = cnt;
    stackp->frma[stackp->frms++].offset = offset;
}

static inline void ss_stack_pop(struct ss_stack *stackp,
        char ***strap, int *cntp, int *offsetp)
{
    *strap = stackp->frma[--stackp->frms].stra;
    *cntp = stackp->frma[stackp->frms].cnt;
    *offsetp = stackp->frma[stackp->frms].offset;
}

static inline void strp_swap(char **p1, char **p2)
{
    char *tp;
    tp = *p1;
    *p1 = *p2;
    *p2 = tp;
}

static inline int smaller(int i, int j)
{
    return i < j ? i : j;
}

static inline int greater(int i, int j)
{
    return i > j ? i : j;
}

void fast_str_sort(char **stra, int cnt)
{
    struct ss_stack stack;
    int i, j, p, q;
    int offset;

    if (cnt <= 1)
        return;
    if ((stack.frma =
                malloc(ceil(log2(cnt)) * sizeof (struct ss_frame) * 2 + 1))
                == NULL) {
        perror("malloc");
        exit(1);
    }
    stack.frms = 0;
    ss_stack_push(&stack, stra, cnt, 0);
continue_main_loop:
    while (!ss_stack_empty(&stack)) {
        char c, v;
        ss_stack_pop(&stack, &stra, &cnt, &offset);
        i = 0;
        j = cnt - 1;
        /* pick the last element as the partition one */
        while (i < j && stra[j][offset] == '\0')
            strp_swap(&stra[i++], &stra[j]);
        if (i == j)
            goto continue_main_loop;
        cnt -= i;
        stra += i;
        v = stra[cnt-1][offset];
        i = 0, q = j = cnt-2, p = 0; 
continue_traver_loop:
        while (1) {
            while (i < j) {
                c = stra[i][offset];
                if (c == '\0') {
                    char *orig_0, *orig_p;
                    orig_0 = stra[0];
                    orig_p = stra[p];
                    stra[0] = stra[i];
                    if (p != 0)
                        stra[p] = orig_0;
                    if (p != i)
                        stra[i] = orig_p;
                    if (--cnt <= 1)
                        goto continue_main_loop;
                    stra++; j--; q--;
                } else if (c < v)
                    i++;
                else if (c > v)
                    break;
                else // c == v
                    strp_swap(&stra[i++], &stra[p++]);
            }
            while (i < j) {
                c = stra[j][offset];
                if (c == '\0') {
                    strp_swap(&stra[i], &stra[j]);
                    char *orig_0, *orig_p;
                    orig_0 = stra[0];
                    orig_p = stra[p];
                    stra[0] = stra[i];
                    if (p != 0)
                        stra[p] = orig_0;
                    if (p != i)
                        stra[i] = orig_p;
                    if (--cnt <= 1)
                        goto continue_main_loop;
                    stra++; j -= 2; q--;
                    goto continue_traver_loop;
                } else if (c > v)
                    j--;
                else if (c < v) {
                    strp_swap(&stra[i++], &stra[j--]);
                    goto continue_traver_loop;
                } else // c == v
                    strp_swap(&stra[j--], &stra[q--]);
            }
            if (i == j) {
                if (stra[i][offset] > v)
                    j--;
                else if (stra[j][offset] < v)
                    i++;
                else {
                    strp_swap(&stra[p++], &stra[i++]);
                }
            }
            // arrive at here, we can figure out:
            // 1. every p <= x < i, stra[x][offset] < v
            // 2. every j < x <= q, stra[x][offset] > v
            int l_len, r_len;
            l_len = i - p;
            for (int t1 = 0, t2 = i - 1, amt = smaller(p, l_len); amt > 0; amt--)
                strp_swap(&stra[t1++], &stra[t2--]);
            r_len = q - j;
            for (int t1 = j+1, t2 = cnt - 1, amt = smaller(cnt - 1 - q, r_len); amt > 0; amt--)
                strp_swap(&stra[t1++], &stra[t2--]);
            if (cnt - l_len - r_len > 1)
                ss_stack_push(&stack, &stra[l_len], cnt - l_len - r_len, offset+1);
            if (r_len > l_len) {
                if (r_len > 1) {
                    ss_stack_push(&stack, &stra[cnt - r_len], r_len, offset);
                    if (l_len > 1)
                        ss_stack_push(&stack, stra, l_len, offset);
                }
            } else {
                if (l_len > 1) {
                    ss_stack_push(&stack, stra, l_len, offset);
                    if (r_len > 1)
                        ss_stack_push(&stack, &stra[cnt - r_len], r_len, offset);
                }
            }
            goto continue_main_loop;
        }
    }
    free(stack.frma);
}

