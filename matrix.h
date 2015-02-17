#include <vector>
#include <algorithm>
#include <functional>
#include <memory>

//reserve matrix rows cols id determinant inverse

template<typename T>
class matrix {
 public:
  typedef std::vector<T> vt;
  typedef std::vector<vt> vvt;

  matrix(int rows, int cols)
      : mat_(rows, vt(cols)),
        rows_(rows),
        cols_(cols),
        id_cache_(NULL) {
  }

  template<typename Iterator>
  matrix(int rows, int cols, Iterator data)
      : mat_(rows, vt(cols)),
        rows_(rows),
        cols_(cols),
        id_cache_(NULL) {
    for (int j = 0; j < rows; j++) {
      for (int i = 0; i < cols; i++) {
        mat_[j][i] = *data;
        ++data;
      }
    }
  }

  virtual ~matrix() {
    if (id_cache_ != NULL) {
      delete id_cache_;
    }
  }

  vt& operator[](int row) {
    return mat_[row];
  }

  const vt& operator[](int row) const {
    return mat_[row];
  }

  int rows() const {
    return rows_;
  }

  int cols() const {
    return cols_;
  }

  matrix<T> operator+(const matrix<T>& b) const {
    matrix<T> ans(rows_, cols_);
    for (int j = 0; j < rows_; j++) {
      std::transform(mat_[j].begin(), mat_[j].end(), b[j].begin(),
                     ans[j].begin(), std::plus<T>());
    }
    return ans;
  }

  matrix<T> operator*(const matrix<T>& b) const {    
    matrix<T> ans(rows_, b.cols());
    mul(*this, b, ans);
    return ans;
  }

  matrix<T> id() const {
    if (id_cache_ != NULL) {
      return *id_cache_;
    }
    id_cache_ = new matrix<T>(rows_, cols_);
    for (int i = 0; i <rows_; i++)
      (*id_cache_)[i][i] = 1;
    return *id_cache_;
  }

  template <typename S>
  matrix<T> power(S n) const {
    if (n == 0) return id();
    if (n == 1) return *this;

    matrix<T> b(*this);
    matrix<T> ans(id());
    matrix<T> temp(id());
    for (S exp = n; exp; exp >>= 1) {
      if (exp & 1) {
        mul(ans, b, temp);
        ans.mat_.swap(temp.mat_);
      }
      mul(b, b, temp);
      b.mat_.swap(temp.mat_);
    }
    return ans;
  }
  
  bool operator==(const matrix<T>& b) const {
    for (int j = 0; j < rows_; j++) {
      if (!std::equal(mat_[j].begin(), mat_[j].end(), b[j].begin())) {
        return false;
      }
    }
    return true;
  }

  bool operator!=(const matrix<T>& b) const {
    return !(*this == b);
  }

  T determinant() {
    if (rows_ != cols_) {
      return 0;
    }
    if (rows_ == 1) {
      return mat_[0][0];
    }
    if (rows_ == 2) {
      return mat_[0][0] * mat_[1][1] - mat_[0][1] * mat_[1][0];
    }
    T ans = 0, sign = 1;
    for (int e = 0; e < rows_; e++) {
      matrix<T> m(rows_ - 1, cols_ - 1);
      for (int j = 0, jj = 0; j < rows_; j++) {
        if (j == e) continue;
        for (int i = 1; i < cols_; i++) {
          m[jj][i - 1] = mat_[j][i];
        }
        jj++;
      }
      ans += sign * mat_[e][0] * m.determinant();
      sign = -sign;
    }
    return ans;
  }

  std::pair<bool, matrix<T> > inverse() {
    int n = rows_;
    std::vector<std::vector<T> > mat(n, std::vector<T>(2 * n, T(0)));
    for (int j = 0; j < rows_; j++) {
      for (int i = 0; i < cols_; i++) {
        mat[j][i] = mat_[j][i];
      }
      mat[j][n + j] = T(1);
    }
    for (int j = 0; j < n; j++) {
      // find pivot.
      int pivot = j;
      for (int k = j + 1; k < n; k++) {
        if (abs(mat[k][j]) > abs(mat[pivot][j])) {
          pivot = k;
        }
      }
      if (mat[pivot][j] == T(0)) {
        return std::make_pair(false, *this);
      }
      // swap pivot if needed.
      if (pivot != j) {
        mat[pivot].swap(mat[j]);
      }
      // normalize line.
      T inv = T(1) / mat[j][j];
      for (int i = j; i < 2 * n; i++)
        mat[j][i] = mat[j][i] * inv;
      // subtract from all others.
      for (int k = 0; k < n; k++) {
        if (k == j || mat[k][j] == 0) continue;      
        T x = mat[k][j];
        for (int i = 0; i < 2 * n; i++) {
          mat[k][i] = mat[k][i] - x * mat[j][i];
        }
      }
    }
    matrix<T> ans(n, n);
    for (int j = 0; j < n; j++) {
      for (int i = 0; i < n; i++) {
        ans[j][i] = mat[j][n + i];
      }
    }
    return std::make_pair(true, ans);
  }

 private:
  void mul(const matrix<T>& a, const matrix<T>& b, matrix<T>& ans) const {
    for (int j = 0; j < a.rows(); j++) {
      for (int i = 0; i < b.cols(); i++) {
        ans[j][i] = 0;
        for (int k = 0; k < a.cols(); k++) {
          ans[j][i] = ans[j][i] + a[j][k] * b[k][i];
        }
      }
    }
  }
 
  vvt mat_;
  int rows_, cols_;
  mutable matrix<T>* id_cache_;
};
