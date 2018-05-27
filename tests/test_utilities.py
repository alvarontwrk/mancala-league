from app.utilities import allowed_file, render_dataframe, extract_numbers
import pandas as pd


class TestUtilities(object):
    def test_extract_numbers(self):
        string = 'Test 1 string 4 5'
        assert extract_numbers(string) == [1, 4, 5]

    def test_allowed_file(self):
        file1 = 'test.txt'
        file2 = 'test.h'
        file3 = 'test.hpp'
        file4 = 'test.cpp'
        assert allowed_file(file1) is False
        assert allowed_file(file2) is True
        assert allowed_file(file3) is True
        assert allowed_file(file4) is True

    def test_render_dataframe(self):
        r = render_dataframe(pd.DataFrame(columns=['Test']))
        assert r
