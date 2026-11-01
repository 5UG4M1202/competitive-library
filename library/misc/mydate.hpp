#pragma once

#include <cassert>
#include <limits>

// The user reports creating this legacy @mydate snippet with AI assistance.
// Copyright 2026 rrrriki. Copyrightable human-authored contributions are
// licensed under Apache-2.0; this provenance report does not guarantee rights
// in every generated fragment. See LICENSE-SCOPE.md and THIRD_PARTY_NOTICES.md.

/**
 * @brief 日付計算ライブラリ
 *
 * 使い方
 *
 * - mydate(y,m,d) md; := y年m月d日の日付を表す構造体
 *
 * - mydate::daysInMonth(y,m):= y年m月のその月の日数を返す
 *
 * - mydate::isValidDate(y,m,d):= y年m月d日が妥当な日付かを判定
 *
 * - md.addDays(d):= d日後の日付を返す
 *
 * - md.subDays(d):= d日前の日付を返す
 *
 * - md.addMonths(m):= mヶ月後の日付を返す
 *
 * - md.dayOfWeek():= 曜日を返す
 *
 * @note 0=月, 1=火, 2=水, 3=木, 4=金, 5=土, 6=日
 */
struct mydate {
 private:
  // 正の除数に対する床除算。
  static long long floorDiv(long long x, long long divisor) {
    return x / divisor - (x % divisor < 0);
  }

 public:
  int year, month, day;
  mydate(int year, int month, int day) : year(year), month(month), day(day) {}
  mydate() : year(0), month(0), day(0) {}
  //-----------------------------------------------------------------------------
  // 比較演算子
  //-----------------------------------------------------------------------------
  /**
   * @brief 日付が引数dより早い(前の日付)かを判定
   * @param d 比較対象の日付
   * @return true: this < d
   */
  bool operator<(const mydate &d) const {
    if (year != d.year) {
      return year < d.year;
    }
    if (month != d.month) {
      return month < d.month;
    }
    return day < d.day;
  }

  /**
   * @brief 日付が引数dより遅い(後の日付)かを判定
   * @param d 比較対象の日付
   * @return true: this > d
   */
  bool operator>(const mydate &d) const {
    if (year != d.year) {
      return year > d.year;
    }
    if (month != d.month) {
      return month > d.month;
    }
    return day > d.day;
  }

  /**
   * @brief 日付が等しいかを判定
   * @param d 比較対象の日付
   * @return true: 同じ日付
   */
  bool operator==(const mydate &d) const {
    return (year == d.year && month == d.month && day == d.day);
  }

  /**
   * @brief 日付が等しくないかを判定
   * @param d 比較対象の日付
   * @return true: 異なる日付
   */
  bool operator!=(const mydate &d) const {
    return !(*this == d);
  }

  /**
   * @brief 日付が引数d以下かを判定
   * @param d 比較対象の日付
   * @return true: this <= d
   */
  bool operator<=(const mydate &d) const {
    return (*this < d) || (*this == d);
  }

  /**
   * @brief 日付が引数d以上かを判定
   * @param d 比較対象の日付
   * @return true: this >= d
   */
  bool operator>=(const mydate &d) const {
    return (*this > d) || (*this == d);
  }

  //-----------------------------------------------------------------------------
  // 静的関数: 月の日数判定など
  //-----------------------------------------------------------------------------

  /**
   * @brief 指定された年月の月の日数を返す
   *
   * @param y 年
   * @param m 月(1～12)
   * @return その (年,月) の日数
   */
  static int daysInMonth(int y, int m) {
    switch (m) {
      case 2:  // 2月
        return ((y) % 4 == 0 && ((y) % 100 != 0 || (y) % 400 == 0)) ? 29 : 28;
      case 4:   // 4月
      case 6:   // 6月
      case 9:   // 9月
      case 11:  // 11月
        return 30;
      default:
        return 31;
    }
  }

  /**
   * @brief (year,month,day) が妥当な日付かチェックする
   *
   * @param y 年
   * @param m 月(1～12)
   * @param d 日(1～31)
   * @return true: 妥当な日付, false: 範囲外
   */
  static bool isValidDate(int y, int m, int d) {
    if (m < 1 || m > 12) return false;
    int dim = daysInMonth(y, m);
    return (d >= 1 && d <= dim);
  }

  //-----------------------------------------------------------------------------
  // 静的関数: JDN との相互変換
  //-----------------------------------------------------------------------------

  /**
   * @brief 日付を Julian Day Number (ユリウス通日) に変換 (プロレプティック・グレゴリオ暦)
   *
   * JDN = 0 は紀元前 4713 年 1 月 1 日（ユリウス暦換算）正午を基準とし、実際の曜日では「月曜日」に対応する。
   * この実装では midnight (0:00) ベースとし、計算式は広く知られたアルゴリズムを用いる。
   *
   * @param y 年
   * @param m 月(1～12)
   * @param d 日(1～31)
   * @return JDN (long long)
   */
  static long long toJulianDay(int y, int m, int d) {
    // 一般的なアルゴリズム(エラー対策省略、月日補正など)
    int a = (14 - m) / 12;
    long long yy = static_cast<long long>(y) + 4800 - a;
    int mm = m + 12 * a - 3;

    long long jd = static_cast<long long>(d) + ((153LL * mm + 2) / 5) + 365LL * yy + floorDiv(yy, 4) - floorDiv(yy, 100) + floorDiv(yy, 400) - 32045LL;

    return jd;
  }

  /**
   * @brief JDN (Julian Day Number) から (year, month, day) を復元 (プロレプティック・グレゴリオ暦)
   *
   * @param jd JDN (long long)
   * @return 復元された mydate
   */
  static mydate fromJulianDay(long long jd) {
    // 参考: 「暦計算アルゴリズム」(Jean Meeus)などで紹介される標準的な式
    assert(jd >= toJulianDay(std::numeric_limits<int>::min(), 1, 1));
    assert(jd <= toJulianDay(std::numeric_limits<int>::max(), 12, 31));
    long long a = jd + 32044;
    long long b = floorDiv(4 * a + 3, 146097);
    long long c = a - floorDiv(146097 * b, 4);

    long long d = (4 * c + 3) / 1461;
    long long e = c - (1461 * d) / 4;
    long long m = (5 * e + 2) / 153;

    long long day = e - (153 * m + 2) / 5 + 1;
    long long month = m + 3 - 12 * (m / 10);
    long long year = 100 * b + d - 4800 + (m / 10);

    return mydate(static_cast<int>(year),
                  static_cast<int>(month),
                  static_cast<int>(day));
  }

  //-----------------------------------------------------------------------------
  // 日付演算
  //-----------------------------------------------------------------------------

  /**
   * @brief this の日付に対して days 日だけ加算した結果を返す
   *
   * @param days 加算する日数(負の値なら減算)
   * @return 新しい日付 (this は変更しない)
   */
  mydate addDays(long long days) const {
    long long jd = toJulianDay(year, month, day);
    assert(days >= toJulianDay(std::numeric_limits<int>::min(), 1, 1) - jd);
    assert(days <= toJulianDay(std::numeric_limits<int>::max(), 12, 31) - jd);
    return fromJulianDay(jd + days);
  }

  /**
   * @brief this の日付に対して days 日だけ減算した結果を返す
   *
   * @param days 減算する日数
   * @return 新しい日付
   */
  mydate subDays(long long days) const {
    long long jd = toJulianDay(year, month, day);
    assert(days >= jd - toJulianDay(std::numeric_limits<int>::max(), 12, 31));
    assert(days <= jd - toJulianDay(std::numeric_limits<int>::min(), 1, 1));
    return fromJulianDay(jd - days);
  }

  /**
   * @brief this の (year, month) をベースに months ヶ月だけ加算した結果を返す
   *
   * @note 日が 31 日などで翌月に存在しない場合は「翌月の月末」に合わせる
   *
   * @param months 加算する月数(負の値なら減算)
   * @return 新しい日付
   */
  mydate addMonths(long long months) const {
    // year, month を合算してトータルの「通算月数」に変換
    // 例: (year=2020, month=1) -> (2020*12 + 0) = 24240
    long long totalMonths = static_cast<long long>(year) * 12 + (month - 1);
    assert(months >= static_cast<long long>(std::numeric_limits<int>::min()) * 12 - totalMonths);
    assert(months <= static_cast<long long>(std::numeric_limits<int>::max()) * 12 + 11 - totalMonths);
    totalMonths += months;

    // 加算後を逆変換
    long long newYear = floorDiv(totalMonths, 12);
    long long newMonth = totalMonths - newYear * 12 + 1;

    // 日が新しい月の日数を超える場合は月末に合わせる
    int dim = daysInMonth(static_cast<int>(newYear), static_cast<int>(newMonth));
    int newDay = (day > dim) ? dim : day;

    return mydate(static_cast<int>(newYear),
                  static_cast<int>(newMonth),
                  newDay);
  }

  //-----------------------------------------------------------------------------
  // 曜日判定
  //-----------------------------------------------------------------------------

  /**
   * @brief 曜日を整数で返す (0=月曜日, 1=火曜日, ... 6=日曜日)
   *
   * - JDN=0 が「月曜日」であることから、
   *   `dayOfWeek = (JDN % 7 + 7) % 7` の結果が 0 ～ 6 となるようにしている。
   *
   * - 実際の歴史的日付は厳密にはユリウス暦→グレゴリオ暦の切り替え日を挟むが、
   *   ここではプロレプティック・グレゴリオ暦で一貫して計算する。
   *
   * @return 0=月, 1=火, 2=水, 3=木, 4=金, 5=土, 6=日
   */
  int dayOfWeek() const {
    long long jd = toJulianDay(year, month, day);
    // 「(jd % 7 + 7) % 7」で負数対策
    // JDN=0 を 0 (月) に対応させるため、このまま余りを取る
    int w = static_cast<int>((jd % 7LL + 7LL) % 7LL);
    return w;
  }
  /// @brief 日付の差を求める
  long long operator-(const mydate &d) const { return toJulianDay(year, month, day) - toJulianDay(d.year, d.month, d.day); }
};
