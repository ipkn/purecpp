#include <sstream>
#include <vector>

using namespace std;

ostringstream cerrmsgs;


struct TestCase
{
	bool success;
	string testName;
	ostringstream cmsg;
	TestCase(string&& testName)
		: success(true), testName(testName)
	{
	}
	~TestCase()
	{
	}

	template <typename T1, typename T2>
	void assertEqual(T1 expected, T2 actual)
	{
		if (expected != actual)
		{
			cmsg << "assertEqual fail: expected " << expected << " but " << actual << endl;
			success = false;
		}
	}

	void assert(bool v, string&& msg)
	{
		if (!v)
		{
			success = false;
			cmsg << msg << endl;
		}
	}

	bool Test()
	{
		DoTest();
		if (success)
			cout << '.';
		else
		{
			cout << 'F';
			cerrmsgs << "FAILED TEST " << testName << endl;
			cerrmsgs << cmsg.str();
			cerrmsgs << endl;
		}
		return success;
	}
private:
	virtual void DoTest() = 0;
};

vector<TestCase*> testCases;

void run()
{
	int total = 0;
	int success = 0;
	for(auto it = testCases.begin(); it != testCases.end(); ++it)
	{
		total ++; 
		if ((*it)->Test())
			success++;
	}
	cout << endl;
	if (!cerrmsgs.str().empty())
		cout << endl << cerrmsgs.str() << endl;
	cout << success << '/' << total << " tests passed." << endl;
}

#define DISABLED_TEST(x) \
	struct S__##x : public TestCase \
	{ \
		S__##x() : TestCase(#x){} \
		void DoTest(); \
	} inst_S__##x; \
	void S__##x::DoTest()

#define TEST(x) \
	struct S__##x : public TestCase \
	{ \
		S__##x() : TestCase(#x){testCases.push_back(this);} \
		void DoTest();\
	} inst_S__##x; \
	void S__##x::DoTest()
