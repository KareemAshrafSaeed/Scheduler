#ifndef PAIR_
#define PAIR_
class Pair
{
private:
	int IO_R, IO_D;
public:
	Pair(int Io_r = 0, int Io_d = 0) {
		IO_R = Io_r;
		IO_D = Io_d;
	}
	void set_D(int D) {
		IO_D = D;
	}
	void set_R(int R) {
		IO_R = R;
	}
	int get_D() { return IO_D; }
	int get_R() { return IO_R; }

	~Pair();
};

#endif