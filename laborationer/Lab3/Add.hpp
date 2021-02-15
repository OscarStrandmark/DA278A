template<int X, int Y>
struct Add
{
	static const int value = Add<X-1,Y+1>::value;
};

template<int Y>
struct Add<0, Y>
{
	static const int value = Y;
};

int AddFun(int X, int Y)
{
	if (X == 0) return Y;
	else return AddFun(X - 1, Y + 1);
}