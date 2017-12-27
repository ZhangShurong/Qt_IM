//http://www.cnblogs.com/ka200812/archive/2011/08/11/2135607.html
#include <iostream>
#include <cstring>
#include <iostream>
using namespace std;
const int SIZE = 13;
const int mod = 100000000;
int state[1 << SIZE];//每一行最多有2^SIZE个状态
int corn[1 << SIZE];      //地的状态最多SIZE行
int dp[SIZE][1 << SIZE];//动态规划数组
//判断该行是否满足不相邻的条件
bool check_sig_line(int x)
{
    return (x & (x << 1));
}
//判断两行是否冲突
bool check_two_line(int i, int x)
{
    return (corn[i] & state[x]);
}

int main()
{
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
	for (int j = 1; j <= m; j++) {
	    int x;
	    cin >> x;
	    if (x == 0)
		corn[i] += (1 << (j - 1));
	}
    }
    int k = 0;
    for (int i = 0; i < (1 << m); i++) {
	if (!check_sig_line(i))
	    state[k++] = i;
    }
    for (int i = 0; i < k; i++) {
	if (!check_two_line(1, i))
	    dp[1][i] = 1;
    }

    for (int i = 2; i <= n; i++) {
	for (int j = 0; j < k; j++) {
	    if (check_two_line(i, j))	//判断第i行 假如按状态j放牛的话行不行。
		continue;
	    for (int f = 0; f < k; f++) {
		if (check_two_line(i - 1, f))	//剪枝 判断上一行与其状态是否满足
		    continue;
		if (!(state[j] & state[f]))
		    dp[i][j] += dp[i - 1][f];
	    }
	}
    }

    int ans = 0;
    for (int i = 0; i < k; i++) {
	ans += dp[n][i];
	ans %= mod;
    }
    cout << ans;

    return 0;
}
