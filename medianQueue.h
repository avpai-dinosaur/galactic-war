// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#include<queue>

class medianQueue
{
public:
	medianQueue() {
	}

	void push(int n) {
		if (upperQueue.empty()) {
			upperQueue.push(n);
			return;
		}

		if (n >= upperQueue.top()) {
			upperQueue.push(n);
		}
		else {
			lowerQueue.push(n);
		}

		rebalance();
	}

	void rebalance() {
		// If only put one element in at a time and
		// check rebealance each time then difference 
		// in size will never be greater than 2
		if (upperQueue.size() - lowerQueue.size() == 2) {
			lowerQueue.push(upperQueue.top());
			upperQueue.pop();
		}
		else if (lowerQueue.size() - upperQueue.size() == 2) {
			upperQueue.push(lowerQueue.top());
			lowerQueue.pop();
		}
	}

	int getMedian() {
		if (upperQueue.size() == 0 && lowerQueue.size() == 0) {
			return 0;
		}

		if (upperQueue.size() == lowerQueue.size()) {
			return ((upperQueue.top() + lowerQueue.top()) / 2);
		}
		else if (upperQueue.size() > lowerQueue.size()) {
			return upperQueue.top();
		}
		else {
			return lowerQueue.top();
		}
	}

	std::size_t size() {
		return upperQueue.size() + lowerQueue.size();
	}

private:
	std::priority_queue<int, std::vector<int>, std::greater<int>> upperQueue;
	std::priority_queue<int, std::vector<int>, std::less<int>> lowerQueue;
};
