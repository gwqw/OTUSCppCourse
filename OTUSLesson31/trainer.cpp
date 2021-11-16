#include "trainer.h"

#include <iostream>

#include <dlib/clustering.h>
#include <dlib/svm_threaded.h>

//#define CROSS_VALIDATION

using namespace std;
using namespace dlib;

std::vector<double> RealtyTrainer::cluster(const std::vector<sample_type> &samples,
                                           std::size_t cluster_num)
{
    using kernel = rbf_kernel;
    dlib::kcentroid<kernel> kc{kernel{gamma}};
    dlib::kkmeans<kernel> test{kc};

    test.set_number_of_centers(cluster_num);
    std::vector<sample_type> initial_centers;
    pick_initial_centers(cluster_num, initial_centers, samples, test.get_kernel());
    test.train(samples, initial_centers);

    std::vector<double> res;
    res.reserve(samples.size());
    for (const auto& sample : samples) {
        res.push_back(test(sample));
    }
    return res;
}

void RealtyTrainer::train(const std::vector<sample_type>& samples, const std::vector<double> &labels,
        const std::string& filename)
{
    //make the one_vs_one_trainer.
    ovo_trainer trainer;
    // make the binary trainers and set some parameters
    krr_trainer<rbf_kernel> rbf_trainer;
    svm_nu_trainer<poly_kernel> poly_trainer;
    rbf_trainer.set_kernel(rbf_kernel(gamma));
    poly_trainer.set_kernel(poly_kernel(0.1, 1, 2));
    // Now tell the one_vs_one_trainer that, by default, it should use the rbf_trainer
    // to solve the individual binary classification subproblems.
    trainer.set_trainer(rbf_trainer);
    // We can also get more specific.  Here we tell the one_vs_one_trainer to use the
    // poly_trainer to solve the class 1 vs class 2 subproblem.  All the others will
    // still be solved with the rbf_trainer.
    trainer.set_trainer(poly_trainer, 1, 2);
    // Next, if you wanted to obtain the decision rule learned by a one_vs_one_trainer you
    // would store it into a one_vs_one_decision_function.
    one_vs_one_decision_function<ovo_trainer> df = trainer.train(samples, labels);
    // If you want to save a one_vs_one_decision_function to disk, you can do
    // so.  However, you must declare what kind of decision functions it contains.
    one_vs_one_decision_function<ovo_trainer,
            decision_function<poly_kernel>,  // This is the output of the poly_trainer
            decision_function<rbf_kernel>    // This is the output of the rbf_trainer
        > df2;

    // Put df into df2 and then save df2 to disk.  Note that we could have also said
    // df2 = trainer.train(samples, labels);  But doing it this way avoids retraining.
    df2 = df;
    serialize(filename) << df2;

#ifdef CROSS_VALIDATION
    // Now let's do 5-fold cross-validation using the one_vs_one_trainer we just setup.
    // As an aside, always shuffle the order of the samples before doing cross validation.
    // For a discussion of why this is a good idea see the svm_ex.cpp example.
    auto samples_copy = samples;
    auto labels_copy = labels;
    randomize_samples(samples_copy, labels_copy);
    try {
        cout << "cross validation: \n"
             << cross_validate_multiclass_trainer(trainer, samples_copy, labels_copy, 5)
             << endl;
    } catch (const exception& e) {
        cerr << "cross validation fails, results can be invalid" << endl;
        cerr << e.what() << endl;
    }
#endif
}
