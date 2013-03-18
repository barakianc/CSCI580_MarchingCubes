#ifndef __HISTOGRAM_HPP__
#define __HISTOGRAM_HPP__

#include <vector>

using namespace std;

class Histogram
{
private:
	struct node
	{
		float	value;
		node	*next;
	};

public:
	
	vector<float> values;

	// circle queue variables
	node			*root;
	node			*head;
	unsigned int	size;

	// statistics variables
	float			minValue;
	float			maxValue;
	float			average;

	Histogram()
	{
		Histogram(20);
	}

	Histogram(unsigned int numElements)
	{
		size = numElements;
		root = head = NULL;
	}

	~Histogram()
	{
		// delete circle queue
	}

	void Init()
	{
		//head = 0;
		minValue = 0.0f;
		maxValue = 0.0f;
		average = 0.0f;

		head = root = new node[1];
		for (size_t i=0; i<size; i++)
		{
			root->value = 0.0f;
			root->next = new node[1];
			root = root->next;

			values.push_back(0.0f);
		}
		root->next = head;	// complete the loop
		root = head;		// move back to head
	}

	void Push(float val)
	{
		head->value = val;
		IncrementHead();

		Update();
	}

private:

	void IncrementHead()
	{
		head = head->next;
	}

	void Update()
	{
		node *temp = head;
		unsigned int i = 0;
		minValue = maxValue = temp->value;
		average = 0.0f;

		temp = head->next;
		while (temp != head)
		{
			if (temp->value < minValue)
				minValue = temp->value;
			if (temp->value > maxValue)
				maxValue = temp->value;

			average += temp->value;
			
			values[i] = temp->value;
			i++;

			temp = temp->next;
		}

		average /= (float)size;
	}
};

#endif