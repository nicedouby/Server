#include <S_Core.h>
#include <MemStat.h>

int main()
{
	S_Core* MyS_Core = new S_Core();
	MyS_Core->Run();

	while(1);
	return 0;
}