#include <cstdio>
#include <cstring>
#include <limits>
#include <string>

//reserve _fastio fastio word line

class Stdio {
 public:
  int read(char *input_buffer, int size) {
    return fread(input_buffer, 1, size, stdin);
  }
  void write(char *output_buffer, int size) {
    fwrite(output_buffer, 1, size, stdout);
  }
};

template<int BUFSIZE=8192, int STRMAX=20, class CustomIO=Stdio>
class _fastio {
 public:
  _fastio() {
    p = input_buffer = new char[BUFSIZE];
    q = output_buffer = new char[BUFSIZE];
    remaining = CustomIO().read(input_buffer, BUFSIZE);
  }

  ~_fastio() {
    if (q - output_buffer > 0) {
      CustomIO().write(output_buffer, q - output_buffer);
    }
    delete[] input_buffer;
    delete[] output_buffer;
  }

  template<typename T>
  operator T() {
    T input;
    read_integer(input, IsUnsigned<T>());
    return input;
  }

  void word(std::string& input) {
    input.clear();
    update_input();
    skip_space();
    while (*p > 32) {
      input.push_back(*p++);
    }
  }

  void line(std::string& input) {
    input.clear();
    update_input();
    while (*p != 13 && *p != 10 && *p) {
      input.push_back(*p++);
    }
    if (*p == 13 && *(p + 1) == 10) {
      p++;
    }
    p++;
  }

  template<typename T>
  _fastio& operator>>(T& input) {
    return read_integer(input, IsUnsigned<T>());
  }

  template<typename T>
  _fastio& operator<<(const T& output) {
    return write_integer(output, IsUnsigned<T>());
  }

  _fastio& operator<<(std::string output) {
    return write_string(output.c_str());
  }

  _fastio& operator<<(const char output[]) {
    return write_string(output);
  }

  template<typename T>
  void write_absolute_leading_zeros(const T& input, int zeros) {
    static char out[std::numeric_limits<T>::digits10 + 2];
    out[0] = 0;
    char *r = out + 1;
    T temp(input);
    for (int i = 0; i < zeros; i++) {
      *r++ = temp % 10 + '0';
      temp /= 10;
    }
    while (*--r) {
      *q++ = *r;
    }
  }

 private:
  template<bool> class Bool {};
  template<class T> class IsUnsigned : public Bool<(static_cast<T>(-1) > 0)> {};

  char *input_buffer, *output_buffer;
  char *p, *q;
  int remaining;

  _fastio& write_string(const char* output) {
    update_output();
    for (const char* x = output; *x; x++) {
      *q++ = *x;
    }
    return *this;
  }

  template<typename T>
  _fastio& write_integer(const T& output, Bool<true> b) {
    update_output();
    write_absolute(output);
    return *this;
  }

  template<typename T>
  _fastio& write_integer(const T& output, Bool<false> b) {
    update_output();
    if (output < 0) {
      *q++ = '-';
      if (std::numeric_limits<T>::min() == output) {
        write_absolute(-(output + 1));
        ++*(q - 1);
      } else {
        write_absolute(-output);
      }
    } else {
      write_absolute(output);
    }
    return *this;
  }

  template<typename T>
  void write_absolute(const T& input) {
    if (input < 10) {
      *q++ = '0' + input;
      return;
    }
    static char out[std::numeric_limits<T>::digits10 + 2];
    out[0] = 0;
    char *r = out + 1;
    T temp(input);
    while (temp) {
      *r++ = temp % 10 + '0';
      temp /= 10;
    }
    while (*--r) {
      *q++ = *r;
    }
  }

  template<typename T>
  _fastio& read_integer(T& input, Bool<false> b) {
    update_input();
    skip_space();
    if (*p == '-') {
      p++;
      read_absolute(input);
      input = -input;
    } else {
      read_absolute(input);
    }
    return *this;
  }

  template<typename T>
  _fastio& read_integer(T& input, Bool<true> b) {
    update_input();
    skip_space();
    read_absolute(input);
    return *this;
  }

  void update_output() {
    if (q - output_buffer >= BUFSIZE - STRMAX) {
      CustomIO().write(output_buffer, q - output_buffer);
      q = output_buffer;
    }
  }

  void update_input() {
    if (p - input_buffer >= BUFSIZE - STRMAX) {
      int current = p - input_buffer;
      int left = remaining - current;
      memcpy(input_buffer, p, left);
      remaining = left + CustomIO().read(input_buffer + left, BUFSIZE - left);
      p = input_buffer;
    }
  }

  void skip_space() {
    while (*p <= 32) p++;
  }

  template<typename T>
  void read_absolute(T& input) {
    input = *p++ - '0';
    while (*p > 32) {
      input = input * 10 + *p++ - '0';
    }
  }
};

typedef _fastio<> fastio;
