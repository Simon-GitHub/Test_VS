/*
	C++ Primer ��12�� ��̬�ڴ�
	����C++��׼�����ṩ������ָ��shared_ptr
*/

#include <memory>

void test_shared_ptr()
{
	// ��������ָ������ü���
	auto r = make_shared<int>(42);
	int rc = r.use_count(); // 1
	auto q = make_shared<int>(50);
	int qc = q.use_count(); // 1
	r = q;
	int rc2 = r.use_count(); // 2
	int qc2 = q.use_count(); // 2
}