# PyStringLib | لایبرری پردازش رشته شبیه پایتون برای C++

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

📚 **English below**

---

## 📌 معرفی (Introduction - فارسی)

کتابخانه‌ی PyStringLib مجموعه‌ای از توابع مفید برای پردازش رشته‌هاست که در C++ نوشته شده و از رفتارهای پایتونی مثل `slice`, `replace`, `find`, `count` و ... پشتیبانی می‌کند.

هدف این پروژه ارائه‌ی ابزاری ساده برای توسعه‌دهندگان C++ است که بتوانند با الهام از پایتون، کدنویسی راحت‌تری داشته باشند.

---

## ⚙️ روش استفاده (Usage)

```cpp
#include "PyString.h"

PyString py;
std::string text = "pouya";

std::cout << py.revers(text);         // Output: "ayuoP"
std::cout << py.removeVowels(text);   // Output: "py"
std::cout << py.slice(text, 1, 4, 1); // Output: "ouy"
std::cout << py.find(text, "uya");    // Output: 2
```

---

## 🚀 نصب (Installation)

```bash
git clone https://github.com/Pouyazadmehr83/PyStringLib.git
cd PyStringLib
g++ -Iinclude src/PyString.cpp examples/main.cpp -o pystring_demo
./pystring_demo
```

---

## 📄 لایسنس

این پروژه تحت مجوز MIT منتشر شده است.

---

## 🌐 English Version

### 📌 About the Project

PyStringLib is a lightweight C++ library that brings some Python-like string functions into your C++ code, such as `slice`, `replace`, `find`, `count`, and more.

### ⚙️ Usage

```cpp
#include "PyString.h"

PyString py;
std::string text = "pouya";

std::cout << py.revers(text);         // "ayuoP"
std::cout << py.removeVowels(text);   // "py"
std::cout << py.slice(text, 1, 4, 1); // "ouy"
std::cout << py.find(text, "uya");    // 2
```

### 📦 Installation

```bash
git clone https://github.com/Pouyazadmehr83/PyStringLib.git
cd PyStringLib
g++ -Iinclude src/PyString.cpp examples/main.cpp -o pystring_demo
./pystring_demo
```

---

### 📄 License

This project is licensed under the MIT License.
