//
// Created by mateuscmarim on 07/01/2021.
//

#ifndef UFJF_MLTK_BALLTREE_HPP
#define UFJF_MLTK_BALLTREE_HPP

#include "Data.hpp"
#include "DistanceMetric.hpp"

namespace mltk {
    namespace metrics {
        template<typename T, typename Metric = dist::Euclidean<T> >
        class BallTree {

            struct BallNode{
                Metric distance;
                Point<T> center{};
                double radius = 0;
                std::vector<PointPointer<T>> points{};
                BallNode* left = nullptr;
                BallNode* right = nullptr;

                BallNode() = default;
                BallNode(std::vector<PointPointer<T> >& points);
                BallNode(PointPointer<T>& point);
                void compute_center();
                PointPointer<T> find_further_away(Point<T> p);
                void populate_children(PointPointer<T> p, PointPointer<T> q);

                ~BallNode(){
                    if(left) delete left;
                    if(right) delete right;
                }
            };

        private:
            Metric distance;
            BallNode *m_root = nullptr;

        public:
            static size_t nodes_count;
            BallTree() = default;
            BallTree(Data<T>& data);

            void construct(BallNode* root);
            void build(Data<T> &data);
            std::vector<std::pair<PointPointer<T>, double>> knn_search(Point<T> const& t, size_t k);
            void knn_search(Point<T> const& t, size_t k, std::vector<std::pair<PointPointer<T>, double>>& Q, BallNode *child);

            ~BallTree();
        };

        template<typename T, typename Metric>
        size_t BallTree<T, Metric>::nodes_count = 0;
        template<typename T, typename Metric>
        BallTree<T, Metric>::BallTree(Data<T> &data) {

            this->build(data);
            std::clog << "Ball tree built." << std::endl;
        }

        template<typename T, typename Metric>
        void BallTree<T, Metric>::build(Data<T> &data) {
            if(data.getSize() == 0) return;
            auto points = data.getPoints();
            m_root = new BallNode(points);
            construct(m_root);
        }

        template<typename T, typename Metric>
        void BallTree<T, Metric>::construct(BallTree::BallNode *root) {
            if(!root) {
                return;
            }
            std::clog << "npoints: " << root->points.size() << std::endl;
            nodes_count++;

            if(root->points.size() == 1){
//                root->left = new BallNode;
//                root->left->points.push_back(root->points[0]);
//                root->left->center = *root->points[0];
                return;
            }else{
                auto L = root->find_further_away(root->center);
                auto R = (L)?root->find_further_away(*L): nullptr;
                root->populate_children(L, R);

                construct(root->left);
                construct(root->right);

                auto dl = (root->left)?distance(root->left->center, root->center):0;
                auto dr = (root->right)?distance(root->right->center, root->center):0;
                root->radius = (dl > dr)?dl:dr;
            }
            return ;
        }

        template<typename T, typename Metric>
        void BallTree<T, Metric>::knn_search(Point<T> const& t, size_t k, std::vector<std::pair<PointPointer<T>, double>>& Q, BallNode *root){
            if(!root) return;
            std::cout << root->points.size() << " " << distance(t, *root->points.front()) << std::endl;
            if((distance(t, root->center) - root->radius) >= distance(t, *Q.front().first)){
                return ;
            }else if(root->points.size()==1){
                for(auto const& p: root->points){
                    auto dist = distance(t, *p);
                    auto dist_t_q = distance(t, *Q.front().first);
                    std::cout << dist << " ";
                    if(!Q.empty()){
                        std::cout << dist_t_q << " " << (dist < dist_t_q) << std::endl;
                    }else std::cout << std::endl;
                    if(dist < dist_t_q){
                        std::clog << "heap size: " << Q.size() << std::endl;
                        Q.push_back(std::make_pair(p, dist));
                        std::push_heap(Q.begin(), Q.end(), [](auto const& a, auto const& b){
                            return a.second > b.second;
                        });
                        if(Q.size() >= k){
                            std::clog << "pop from heap" << std::endl;
                            std::pop_heap(Q.begin(), Q.end(), [](auto const& a, auto const& b){
                                return a.second > b.second;
                            });
                            Q.pop_back();
                        }
                    }
                }
            }else{
                double dleft = std::numeric_limits<double>::infinity();
                if(root->left) dleft = distance(root->left->center, t);
                double dright = std::numeric_limits<double>::infinity();
                if(root->right) dright = distance(root->right->center, t);
                if(dleft > dright){
                    knn_search(t, k, Q, root->right);
                    std::cout << "esquerda" << std::endl;
                    knn_search(t, k, Q, root->left);
                }else{
                    knn_search(t, k, Q, root->left);
                    std::cout << "direita" << std::endl;
                    knn_search(t, k, Q, root->right);
                }
            }
            return;
        }

        template<typename T, typename Metric>
        std::vector<std::pair<PointPointer<T>, double> > BallTree<T, Metric>::knn_search(Point<T> const& t, size_t k) {
            std::vector<std::pair<PointPointer<T>, double>> Q;
            for(size_t i = 0; i < k; i++){
                Q.push_back(std::make_pair(m_root->points[i], distance(t, *m_root->points[i])));
                std::push_heap(Q.begin(), Q.end(), [](auto const& a, auto const& b){
                    return a.second > b.second;
                });
            }
            knn_search(t, k, Q, m_root);
            std::sort(Q.begin(), Q.end(), [](auto const& a, auto const& b){
                return a.second < b.second;
            });
            return Q;
        }

        template<typename T, typename Metric>
        BallTree<T, Metric>::~BallTree() {
            delete m_root;
        }

        template<typename T, typename Metric>
        BallTree<T, Metric>::BallNode::BallNode(PointPointer<T>& point) {
            if(!point) return;
            this->center = *point;
        }

        template<typename T, typename Metric>
        BallTree<T, Metric>::BallNode::BallNode(std::vector<PointPointer<T> >& _points) {
            if(_points.empty()) return;
            this->points = _points;
            this->compute_center();
        }

        template<typename T, typename Metric>
        void BallTree<T, Metric>::BallNode::populate_children(PointPointer <T> l, PointPointer <T> r) {
            if(!l && !r) return;
            std::vector<PointPointer<T>> left_set;
            std::vector<PointPointer<T>> right_set;
            for(auto const& p: points){
                if(l && (!r || distance(*l, *p) >= distance(*r, *p))){
                    if(*p != *l) left_set.push_back(p);
                }else{
                    if(*p != *r) right_set.push_back(p);
                }
            }
            if(!left_set.empty()) this->left = new BallNode(left_set);
            if(!right_set.empty()) this->right = new BallNode(right_set);
        }

        template<typename T, typename Metric>
        PointPointer<T> BallTree<T, Metric>::BallNode::find_further_away(Point<T> p) {
            if(points.empty()) return nullptr;
            std::vector<double> distances(points.size());
            std::transform(points.begin(), points.end(), distances.begin(),
                           [&p, this](const std::shared_ptr<Point<T> > q) {
                               return distance(p, *q);
                           });
            auto pos_max_dis = std::max_element(distances.begin(), distances.end())-distances.begin();
            if(distances[pos_max_dis] == 0)
                return nullptr;
            return points[pos_max_dis];
        }

        template<typename T, typename Metric>
        void BallTree<T, Metric>::BallNode::compute_center() {
            if(points.empty()) return;
            this->center.resize(points[0]->size());
            for(auto const& point: points){
                std::vector<double> distances(points.size());
                this->center += *point;
            }
            this->center /= points.size();
        }
    }
}

#endif //UFJF_MLTK_BALLTREE_HPP