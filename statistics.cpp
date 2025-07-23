#include <iostream>
#include <limits>
#include <cmath>

class IStatistics {
public:
	virtual ~IStatistics() {}

	virtual void update(double next) = 0;
	virtual double eval() const = 0;
	virtual const char * name() const = 0;
};

class Min : public IStatistics {
public:
	Min(const Min&) = delete;
	Min& operator=(const Min&) = delete;
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
	Max(const Max&) = delete;
	Max& operator=(const Max&) = delete;
	Max() : m_max_{std::numeric_limits<double>::lowest()} {
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
	Mean(const Mean&) = delete;
	Mean& operator=(const Mean&) = delete;
	Mean() : m_mean_{0.0}, m_cnt_{0} {
	}

	void update(double next) override {
		m_cnt_ += (m_cnt_ < WINDOW_SIZE) ? 1 : 0;
		m_mean_ += (next - m_mean_) / m_cnt_;
	}

	double eval() const override {
		return (m_cnt_ > 0) ? m_mean_ : 0.0;
	}

	const char * name() const override {
		return "mean";
	}

private:
	static constexpr unsigned int WINDOW_SIZE = 16384;
	double m_mean_;
	unsigned int m_cnt_;
};

class Std : public IStatistics {
public:
	Std(const Std&) = delete;
	Std& operator=(const Std&) = delete;
	Std() : m_disp_{0.0}, m_mean_{0.0}, m_cnt_{0} {
	}

	void update(double next) override {
		double delta = next - m_mean_;
		m_cnt_ += (m_cnt_ < WINDOW_SIZE) ? 1 : 0;
		m_mean_ += delta / m_cnt_;
		m_disp_ += delta * (next - m_mean_); // delta2 вычисляется здесь
	}

	double eval() const override {
		return (m_cnt_ > 1) ? std::sqrt(m_disp_ / m_cnt_) : 0.0;
	}

	const char * name() const override {
		return "std";
	}

private:
	static constexpr unsigned int WINDOW_SIZE = 16384;
	double m_disp_;
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