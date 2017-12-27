#include <iostream>
#define DEBUG
using namespace std;
const int SIZE = 13;
const int MOD = 100000000;
static int state[1 << SIZE];//每一行最多有2^SIZE个状态
static int corn[SIZE];      //地的状态最多SIZE行
static int dp[SIZE][1 << SIZE];//动态规划数组

//判断该行是否满足不相邻的条件
bool check_sig_line(int s)
{
    return (s&(s<<1));
}
//判断两行是否冲突
bool check_two_line(int x, int y)
{
    return (corn[x]&state[y]);
}
int main()
{
    int m, n;
    cin >> m >> n;
    for (int i=1;i<=m;i++) {
        for (int j=0;j<n;j++) {
            int x;
            cin >> x;
            if (x==0)
                corn[i]+=(1<<j);
        }
        
    }
    //init 初始化第一行的状态
    int k=0;
    for(int i=0;i<(1<<n);i++){
        if(!check_sig_line(i)){
            state[k++]=i;
        }
    }
    
    for(int i=0;i<k;i++){
        if(!check_two_line(0,i)) {
            dp[1][i]=1;
        }
    }

    for(int i=2;i<=m;i++) {
        for(int j=0;j<k;j++) {
            if(check_two_line(i,j)) 
                continue;
            for(int f=0;f<k;f++) {
                if(check_two_line(i-1,f))   
                    continue;
                if(!(state[j]&state[f]))
                    dp[i][j]+=dp[i-1][f];
            }
        }
    }

    int ans=0;
    for(int i=0;i<k;i++){
        ans+=dp[m][i];
        ans%=MOD;
    }
    cout << ans << endl;
    return 0;
}