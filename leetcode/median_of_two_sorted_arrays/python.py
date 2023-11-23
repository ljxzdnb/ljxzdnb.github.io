    def findMedianSortedArrays(self, nums1, nums2):
        """
        :type nums1: List[int]
        :type nums2: List[int]
        :rtype: float
        """
        s = nums1 + nums2
        s.sort()
        n = len(s) // 2
        if len(s)%2:
            return s[n]
        else:
            return (s[n-1]+s[n])/2
