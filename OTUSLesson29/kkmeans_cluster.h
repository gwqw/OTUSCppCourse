#pragma once

#include <vector>
#include <memory>

#include <dlib/clustering.h>

// Here we declare that our samples will be 2 dimensional column vectors.
// (Note that if you don't know the dimensionality of your vectors at compile time
// you can change the 2 to a 0 and then set the size at runtime)
using sample_type = dlib::matrix<double,2,1>;

// Now we are making a typedef for the kind of kernel we want to use.  I picked the
// radial basis kernel because it only has one parameter and generally gives good
// results without much fiddling.
//using kernel_type = dlib::radial_basis_kernel<sample_type>;
//using kernel_type = dlib::polynomial_kernel<sample_type>;
//using kernel_type = dlib::sigmoid_kernel<sample_type>;
//using kernel_type = dlib::linear_kernel<sample_type>;
//using kernel_type = dlib::histogram_intersection_kernel<sample_type>;

enum class ClusterType {RADIAL, POLYNOMIAL, SIGMOID, LINEAR, HISTOGRAM};

class ICluster {
public:
    virtual ~ICluster() = default;

    virtual std::vector<int> search_clusters(int cluster_count,
                                  const std::vector<sample_type>& samples) = 0;
    virtual std::vector<int> search_spectral_clusters(int cluster_count,
                                  const std::vector<sample_type>& samples) = 0;
};

template <typename kernel_type>
class Cluster : public ICluster {
public:
    template <typename ... Args>
    explicit Cluster(double tolerance, std::size_t max_dict_size, Args&& ... args)
    : kc(kernel_type{std::forward<Args>(args)...}, tolerance, max_dict_size),
      test(kc)
    {}

    std::vector<int> search_clusters(int cluster_count,
                             const std::vector<sample_type>& samples) override;

    std::vector<int> search_spectral_clusters(int cluster_count,
                             const std::vector<sample_type>& samples) override;
private:
    dlib::kcentroid<kernel_type> kc;
    dlib::kkmeans<kernel_type> test;
};

std::unique_ptr<ICluster> makeCluster(ClusterType ctype, double tolerance,
                                      std::size_t max_dict_size, double gamma, double coef, double degree);

template <typename kernel_type>
std::vector<int> Cluster<kernel_type>::search_clusters(int cluster_count,
                                 const std::vector<sample_type>& samples)
{
    test.set_number_of_centers(cluster_count);
    std::vector<sample_type> initial_centers;
    pick_initial_centers(cluster_count, initial_centers, samples, test.get_kernel());
    test.train(samples, initial_centers);

    std::vector<int> res;
    res.reserve(samples.size());
    for (const auto& sample : samples) {
        res.push_back(test(sample));
    }
    return res;
}

template <typename kernel_type>
std::vector<int> Cluster<kernel_type>::search_spectral_clusters(int cluster_count,
                                          const std::vector<sample_type>& samples)
{
    std::vector<int> res;
    res.reserve(samples.size());

    std::vector<unsigned long> assignments = spectral_cluster(kernel_type{}, samples, cluster_count);
    res = {assignments.begin(), assignments.end()};
    return res;
}