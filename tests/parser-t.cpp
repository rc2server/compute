#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <algorithm>
#include "../parser/RmdParser.hpp"

using std::cout;
using std::endl;
using std::string;

namespace RC2 {
namespace testing {

	TEST(ParserTest, basicRParse)
	{
		string code = R"(x <- c(2,3)
		y <- c("foo", "bar")
		rnorm(x[1])
		plot(rnorm(4))
		)";
		RmdParser parser;
		auto strings = parser.stringsInRCode(code);
		ASSERT_TRUE(std::find(strings.begin(), strings.end(), "foo") != strings.end());
		ASSERT_EQ(std::count(strings.begin(), strings.end(), "bar"), 1);
		ASSERT_NE(std::count(strings.begin(), strings.end(), "fobb"), 1);
	}

	TEST(ParserTest, basicRmdParse)
	{
		string code = R"(
foobar
```{r}
z <- 22
plot(z)
```
barfoo
		)";
		RmdParser parser;
		auto chunks = parser.parseRmdSource(code);
		ASSERT_EQ(chunks.size(), 3);
	}
};
};
