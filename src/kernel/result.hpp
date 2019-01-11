#ifndef RESULT_HPP_
#define RESULT_HPP_

enum [[nodiscard]] ResultState
{
	SUCCESS,
	FAIL
};

template<typename T>
class [[nodiscard]] Result
{
public:
	Result(T result);
	Result();
	bool is_success();

	T result;
	ResultState state;
};

template<typename T>
Result<T>::Result(T result)
	: result(result), state(ResultState::SUCCESS)
{

}

template<typename T>
Result<T>::Result()
	: result(), state(ResultState::FAIL)
{

}

template<typename T>
bool Result<T>::is_success()
{
	return state == ResultState::SUCCESS;
}

#endif
