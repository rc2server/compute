#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <memory>
#include "../parser/RmdParser.hpp"
#include "../common/RC2Utils.hpp"
#include "../parser/Chunk.hpp"
using std::cout;
using std::endl;
using std::string;

namespace RC2 {
namespace testing {

	TEST(ParserTest, chunkIdentifier)
	{
string mdown = R"(
doodle 
*news*
```{r foo}
rnorm(20)
```
something
	
)";
		RmdParser parser;
		auto chunks = parser.parseRmdSource(mdown);
		for(int i=0; i < chunks.size(); ++i)  {
			cout << "chunk " << i << "=" << chunks[i]->chunkIdentifier() << endl;
		}
		
	}
	
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

	TEST(ParserTest, memTest) {
		using std::unique_ptr;
		using std::vector;
		struct MT {
			virtual string contents() const { return str; }
			string str;
			int i;
			MT(string sval, int ival) : str(sval), i(ival) {}
			MT(const std::unique_ptr<MT>& other) : str(other->str), i(other->i) {}
		};
		struct MTT : public MT {
			virtual string contents() const override { return "foobar"; }
		};
		vector<std::unique_ptr<MT> >  vector1;
		vector<MT*> ptrVector;
		unique_ptr<MT> val1(new MT("foo", 21));
		vector1.push_back(std::move(val1));
		unique_ptr<MT> val2(new MT("bar", 44));
		vector1.push_back(std::move(val2));
//		std::cout << val1.get()->contents() << std::endl;
		for (int i = 0; i < vector1.size(); ++i) {
			ptrVector.push_back(vector1[i].get());
			auto x1 = vector1[i].get();
			auto x2 = ptrVector[i];
			ASSERT_EQ(x1->str, x2->str);
		}
}
	
	TEST(ParserTest, basicRmdParse)
	{
		string code = R"(
foobar $\frac{1}{n} \sum_{i=i}^{n} x_{i}$
```{r}
z <- 22
plot(z)
```
barfoo
		)";
		RmdParser parser;
		auto chunks = parser.parseRmdSource(code);
		ASSERT_EQ(chunks.size(), 3);
		// if use chunks[0].get().type() it crashes
		auto ck = chunks[0];
		ASSERT_TRUE(ck->type() == markdown);
		std::cout << ck->description() << std::endl;
		ASSERT_EQ(chunks.size(), 3);
		ASSERT_TRUE(chunks[1]->type() == ChunkType::code);
		ASSERT_TRUE(chunks[2]->type() == markdown);
		
	}
	
}; // end testing namespace
}; // end RC2 namespace
