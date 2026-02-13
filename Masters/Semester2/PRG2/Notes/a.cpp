#include <vector>
#include <iostream>
using namespace std;

class NumArray {
private:
    int n;
    vector<int> st_, A_;
    int left(int p) const { return p << 1; }
    int right(int p) const { return (p << 1) + 1; }

    void build(int p, int i, int j) {
        if (i == j) {
            st_[p] = A_[i];
        } else {
            build(left(p), i, (i + j) / 2);
            build(right(p), (i + j) / 2 + 1, j);
            int p1 = st_[left(p)];
            int p2 = st_[right(p)];
            st_[p] = p1 + p2;
        }
    }

    void update(int p, int L, int R, int i, int val){
      int prev = A_[i];
      int new_s = val - prev;
      if (i > R || i < L) {return;}
      st_[p] += new_s;
      update(left(p), L, (L+R)/2, i, val);
      update(right(p), (L+R)/2 + 1, R, i, val);
    }

    int rsq(int p, int L, int R, int i, int j) {
        if (i > R || j < L) {
            return -1;
        }
        if (i == L && j == R) {
            return st_[p];
        }

        int p1 = rsq(left(p), L, (L+R)/2, i, j);
        int p2 = rsq(right(p), (L+R)/2 + 1, R, i , j);
        if (p1 == -1){
          return p2;
        }
        if (p2 == -1){
          return p1;
        }
        return p1 + p2;
    }

public:
    NumArray(vector<int>& nums) {
        A_ = nums;
        n = nums.size();
        st_.assign(4*n, 0);
        build(1, 0, n - 1);
    }

    void update(int index, int val) {
      update(1, 0, n-1, index, val);
    }

    int sumRange(int left, int right) {
      return rsq(1, 0, n-1, left, right);
    }
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * obj->update(index,val);
 * int param_2 = obj->sumRange(left,right);
 */

int main(void) {
	vector<int> A = {1,2,3,4,5,6,7,8,9,10};
	NumArray narr(A);
	int n = A.size();
	int m = narr.sumRange(0, n-1);
	cout << m << endl;

}
