#include <iostream>
#include <limits>

class IStatistics {
public:
	virtual ~IStatistics() {}

	virtual void update(double next) = 0;
	virtual double eval() const = 0;
	virtual const char * name() const = 0;
};

class Min : public IStatistics {
public:
	Min() : m_min_{std::numeric_limits<double>::max()} {
	}

	void update(double next) override {
		if (next < m_min_) {
			m_min_ = next;
		}
	}

	double eval() const override {
		return m_min_;
	}

	const char * name() const override {
		return "min";
	}

private:
	double m_min_;
};

class Max : public IStatistics {
public:
	Max() : m_max_{std::numeric_limits<double>::min()} {
	}

	void update(double next) override {
		if (next > m_max_) {
			m_max_ = next;
		}
	}

	double eval() const override {
		return m_max_;
	}

	const char * name() const override {
		return "max";
	}

private:
	double m_max_;
};

class Mean : public IStatistics {
public:
	Mean() : m_mean_{0.0}, m_cnt_{0} {
	}

	void update(double next) override {
		m_cnt_ += (m_cnt_ < m_max_cnt) ? 1 : 0;
		m_mean_ += (next - m_mean_) / m_cnt_;
	}

	double eval() const override {
		return (m_cnt_ > 0) ? m_mean_ : 0.0;
	}

	const char * name() const override {
		return "mean";
	}

private:
	const unsigned int m_max_cnt = 10000;
	double m_mean_;
	unsigned int m_cnt_;
};


int main() {

	const size_t statistics_count = 4;
	IStatistics *statistics[statistics_count];

	statistics[0] = new Min{};
	statistics[1] = new Max{};
	statistics[2] = new Mean{};
	statistics[3] = new Std{};

	double val = 0;
	while (std::cin >> val) {
		for (size_t i = 0; i < statistics_count; ++i) {
			statistics[i]->update(val);
		}
	}

	// Handle invalid input data
	if (!std::cin.eof() && !std::cin.good()) {
		std::cerr << "Invalid input data\n";
		return 1;
	}

	// Print results if any
	for (size_t i = 0; i < statistics_count; ++i) {
		std::cout << statistics[i]->name() << " = " << statistics[i]->eval() << std::endl;
	}

	// Clear memory - delete all objects created by new
	for (size_t i = 0; i < statistics_count; ++i) {
		delete statistics[i];
	}

	return 0;
}