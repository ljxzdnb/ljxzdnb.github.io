while 1:
    try:
        n = int(input())

        nums = [list(map(int, input().split())) for _ in range(n)]

        nums = sorted(nums, key=lambda x: x[1], reverse=True)
        print(nums)
        dp = [0] * (n + 1)
        start = 0
        for i, j, m in nums:
            print(i,j,m)
            start = min(j, start)
            dp[j] += ((m + dp[i])//100) * 15

        print(f"{start} {dp[start]}")
    except Exception as e:
        break
