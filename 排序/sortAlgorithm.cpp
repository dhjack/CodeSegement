#include <stdio.h>
#include <stdlib.h>

/*
 * 实现常见的排序算法
 * 算法名字         |   时间复杂度   |  空间复杂度 | 是否稳定
 * 1. 冒泡排序         n^2                  1          是
 * 2. 快速排序         NlgN                 1          否
 * 3. 堆排序           NlgN                 1          否
 * 4. 计数排序         NK                   N+K        是
 */

/*
 * 冒泡排序
 * while 上边界 不为0
 *      for i in (0, 上边界)
 *          比较 i 和 i+1 的元素，如果位置i的元素大于位置i+1的元素，交互之
 *      上边界减1
 *
 * # 一个改进，记录第一次发生交换的地方。然后下次直接在第一次发生交换的地方开始冒泡
 */
void bubbleSort(int *a, size_t len)
{
    int border = len - 2;
    while (border >= 0) {
        for (size_t i = 0; i <= border; ++i) {
            if (a[i] > a[i+1]) {
                a[i] ^= a[i+1];
                a[i+1] ^= a[i];
                a[i] ^= a[i+1];
            }
        }
        --border;
    }
}

/*
 * 快速排序(这个版本的，移动最小最大元素，需要很多mv)
 * middle = (right - left) / 2
 * for i in [left, middle)
 *      if a[i] > a[middle]
 *          temp = a[i]
 *          j = i
 *          while j < middle
 *              a[j] = a[j+1]
 *              j++
 *          a[middle] = temp
 *          --middle
 * for i in (middle, right]
 *      if a[i] <= a[middle]
 *          temp = a[i]
 *          j = i
 *          while j > middle
 *              a[j] = a[j-1]
 *          a[middle] = temp
 *          ++middle
 * sort(left, middle - 1 )
 * sort(middle + 1, right )
 */
void quickSortVersion1(int *a, int left, int right)
{
    if (left >= right)
        return;
    int middle = (right - left) / 2 + left, temp, j;
    //printf("left:%d, right:%d, middle:%d\n", left, right, middle);
    for (size_t i = left; i < middle;) {
        if (a[i] > a[middle]) {
            temp = a[i];
            j = i;
            while (j < middle) {
                a[j] = a[j+1];
                j++;
            }
            a[middle--] = temp;
        } else
            ++i;
    }
    for (size_t i = middle + 1; i <= right;) {
        if (a[i] <= a[middle]) {
            temp = a[i];
            j = i;
            while (j > middle) {
                a[j] = a[j-1];
                j--;
            }
            a[middle++] = temp;
        } else 
            ++i;
    }
    quickSortVersion1(a, left, middle - 1);
    quickSortVersion1(a, middle + 1, right);
}

/*
 * 快速排序 版本2
 * 如果右边界小于左边界，那么直接返回
 * 取中间元素(right - left) / 2 + left为中间值，并同right交换。
 * 设置第一个大于中间值的index为0
 * 遍历[left, right-1], 如果当前值小于中间值，那么同第一个大于中间值交换，然后第一个大于中间值的index加1
 * 把中间值同第一个大于中间值的元素交互
 * 继续排序left到index-1和index+1到right
 */
void quickSortVersion2(int *a, int left, int right)
{
    if (right <= left)
        return;
    int middle = (right - left) / 2 + left;
    int firstMax = 0;
    int temp = a[middle];
    a[middle] = a[right];
    a[right] = temp;
    for (size_t i = 0; i <= right - 1; ++i) {
        if (a[i] < a[right]) {
            temp = a[i];
            a[i] = a[firstMax];
            a[firstMax] = temp;
            ++firstMax;
        }
    }
    temp = a[right];
    a[right] = a[firstMax];
    a[firstMax] = temp;
    quickSortVersion2(a, left, firstMax - 1);
    quickSortVersion2(a, firstMax + 1, right);
}

/*
 * 快速排序
 * 如果右边界小于左边界，那么直接返回
 * 取right为中间值
 * 对于范围[left, right-1], 两头同时向中间逼近。左边的记录第一个大于中间值的index，右边记录第一个小于等于中间值的元素。如果有，交换，交换后继续逼近。直到结束
 *      i = left, j = right - 1
 *      do {
 *          i 逼近 j, 找到第一个大于中间值的元素，break。
 *          j 逼近 i，找到第一个小于等于中间值的元素，break。
 *          如果 i < j 交换。然后++i, --j
 *      } while i <= j
 * 把right同i元素交互
 * 继续排序left到index-1和index+1到right
 */
void quickSort(int *a, int left, int right)
{
    if (right <= left)
        return;
    int i = left, j = right-1, temp;
    do {
        while (i <= j && a[i] <= a[right])
            ++i;
        while (j >= i && a[j] > a[right])
            --j;
        if (i < j) {
            a[i] ^= a[j];
            a[j] ^= a[i];
            a[i] ^= a[j];
            ++i;
            --j;
        }
    } while (i <= j);
    temp = a[right];
    a[right] = a[i];
    a[i] = temp;
    quickSort(a, left, i - 1);
    quickSort(a, i + 1, right);
}


/*
 * 堆排序
 * 1.建堆：
 *      把原始数组作为完全二叉树。从数组尾部遍历到0
 *          对于每一个元素，调用heapify调整
 * 2. heapify. 调整当前节点，使其为右子节点，左子节点中，最小一个元素。
 *      如果（左子节点不存在或者当前节点小于左子节点）或者（左子节点不存在或者当前节点小于左子节点）那么已经符合要求。退出
 *      否则，取子节点中最小一个，和当前节点交换，然后对交换后的子节点，继续调用heapify
 * 3. 排序：
 *      取堆顶元素，把最后一个元素放入堆顶，对堆顶调用heapify处理。这时len缩短一个单位。因为原始堆顶放在最后了。
 * 4. heapify改成不用递归，用迭代
 *      找到左子节点。
 *      如果左子节点有效范围，那么和右子节点比大小，定位到最大的子节点。然后和index比较。
 *          如果比index小，那么可以结束了
 *          否则交换。然后更新index和左子节点，继续迭代
 */

void heapify(int *a, size_t index, size_t len)
{
    int minChildIndex = index * 2 + 1;

    while (minChildIndex < len) {
        if (minChildIndex + 1 < len && a[minChildIndex] < a[minChildIndex+1])
            ++minChildIndex;
        if (a[minChildIndex] <= a[index])
            break;
        a[minChildIndex] ^= a[index];
        a[index] ^= a[minChildIndex];
        a[minChildIndex] ^= a[index];
        index = minChildIndex;
        minChildIndex = index * 2 + 1;
    }
}

void buildHeap(int *a, size_t len)
{
    for (int i = len/2 - 1; i >= 0; --i) {
        heapify(a, i, len);
    }
}

void heapSort(int *a, size_t len)
{
    buildHeap(a, len);
    while (len > 1) {
        a[0] ^= a[len - 1];
        a[len - 1] ^= a[0];
        a[0] ^= a[len - 1];
        heapify(a, 0, --len);
    }
}

/*
 * 计数排序
 * 直接计算每个元素的最后位置，然后用另外一个数组直接写入。
 * 过程：
 *      初始化位置数组
 *          遍历数组，把元素值对应的位置数组加1
 *          然后遍历位置数组，累加。就得到每个元素，最后的位置信息
 *      遍历数组，直接根据位置信息，写入到新的数组中。
 */
void countingSort(int *a, size_t len)
{
    int indexs[10] = {0};
    int *b = (int*)malloc(len * sizeof(int));
    if (!b)
        exit(1);

    for (size_t i = 0; i < len; ++i) {
        ++indexs[a[i]];
    }
    for (size_t i = 1; i < 10; ++i) {
        indexs[i] += indexs[i-1];
    }

    for (int i = len - 1; i >= 0; --i) {
        b[indexs[a[i]] - 1] = a[i];
        --indexs[a[i]];
    }

    for (size_t i = 0; i < len; ++i) {
        a[i] = b[i];
    }
    free(b);
}

/* 
 * 插入排序
 *      从头到尾遍历。
 *          #对应每一个元素，从后往前遍历，如果小于前一个元素，那么交换, 否则跳出。至到结束。
 *          还一种，不是交换。而是直接往后面移动。找到第一个小于等于当前元素的index，那么赋值给index+1
 */
void insertionSort(int *a, size_t len)
{
    int temp,j;
    for (size_t i = 0; i < len; ++i) {
        temp = a[i];
        for (j = i - 1; j >= 0 && a[j] > temp; --j) {
            a[j+1] = a[j];
        }
        a[j+1] = temp;
    }
}

/*
 * 桶排序
 */

/*
 * 归并排序，这个对链表挺友好。但是对数组而言，需要的辅助空间
 *
 * 申请空间b。a指向原始数组
 * 步长 in [1, len/2]
 *      把两个步长的数据归并到b数组
 *          for (s1=0, s2=0+步长 ;  ; s1
 *      交换a b
 *
 * 如果b不是原始数组。那么循环复制下。
 */

int main()
{
    int a[] = {5, 9, 7, 1, 4, 6, 0, 2, 3, 8};
    size_t len = sizeof(a) / sizeof(int);

    //bubbleSort(a, len);
    //quickSort(a, 0, len - 1);
    //heapSort(a, len);
    //countingSort(a, len);
    insertionSort(a, len);

    for (size_t i = 0; i < len; ++i)
        printf("%d ", a[i]);
    printf("\n");
}
