#ifndef CONTROLINPUTSTATUSSET_HPP
#     define CONTROLINPUTSTATUSSET_HPP

#include <array>
using std::array;

#define CONTROLINPUTSTATUSSET_MAXCONTROLS 256

struct ControlInputStatusSet : public array<float, CONTROLINPUTSTATUSSET_MAXCONTROLS>
{
	ControlInputStatusSet();

	using array::operator[];

	float status(size_type ID);

	void toggleCondition(size_type ID);
	void setConditionTrue(size_type ID);
	void setConditionFalse(size_type ID);

	void setConditionORedTrue(size_type ID);//this works with multiple users (not thread safe) as long as they do not call these functions in an alternating pattern (true, false, true, false)
	void setConditionORedFalse(size_type ID);//condition will be true when at least one user wants it to be true (condition=#true calls>#false calls)

	float getStatusAndReset(int indx,float resetTo=0);//not thread safe; reset value
};
#endif /* CONTROLINPUTSTATUSSET_HPP */
