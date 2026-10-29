#!/usr/bin/env python3
"""Regression tests for the IntervalSet definition synchronization check."""

import sys
import unittest

sys.dont_write_bytecode = True
import check_interval_set_snippet_sync as check


class IntervalSetSyncTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        _, cls.snippet = check.load_snippet()

    def test_current_definition_matches(self):
        check.compare_maintained_function(self.snippet)

    def test_changed_definitions_are_rejected(self):
        changes = (
            ("Node comparison", "return r < rhs.r;", "return r <= rhs.r;"),
            ("covered length", "if (a < b) res += b - a;", "if (a < b) res += b - a + 1;"),
            ("contained count", "++cnt;", "--cnt;"),
            ("update", "if (it->val == val) {", "if (it->val != val) {"),
            ("erase", "auto it = S.lower_bound(Node(l, 0, VAL()));",
             "auto it = S.upper_bound(Node(l, 0, VAL()));"),
            ("output literal", 's << "(["', 's << "( ["'),
        )
        for name, before, after in changes:
            with self.subTest(name=name):
                self.assertIn(before, self.snippet)
                changed = self.snippet.replace(before, after, 1)
                with self.assertRaisesRegex(ValueError, "definition differs"):
                    check.compare_maintained_function(changed)


if __name__ == "__main__":
    unittest.main()
