void swap(int* a, int* b) {
    int tmp = *b;
    *b = *a;
    *a = tmp;
}
int partition(int* array, int left, int right) {
    int pivot = left;
    int index = left + 1;
    for (int i = index; i <= right; i++) {
        if (array[i] < array[pivot]) {
            swap(&array[i], &array[index]);
            index++;
        }
    }
    swap(&array[pivot], &array[index-1]);
    return index - 1;
}

void quickSort(int* array, int left, int right) {
    if (left < right) {
        int pi = partition(array, left, right);
        quickSort(array, left, pi - 1);
        quickSort(array, pi + 1, right);
    }
}
double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size){
    int* num3 = (int*)malloc(sizeof(int) * (nums1Size+nums2Size));
    memcpy(num3, nums1, sizeof(int) * nums1Size);
    memcpy(num3 + nums1Size, nums2, sizeof(int) * nums2Size);
    quickSort(num3, 0, nums1Size+nums2Size-1);
    int mid = 0;
    double median = 0.f;
    mid = (nums1Size+nums2Size) / 2;
    if ((nums1Size+nums2Size) % 2 == 0) {
        median = (num3[mid] + num3[mid-1]) / 2.0;
    }else {
        median = num3[mid];
    }
    free(num3);
    return median;
}
