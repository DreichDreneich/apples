#pragma once
#include "2dTree.h"

namespace ApplesGame
{
	bool compareX(const Apple& a, const Apple& b) {
		return a.position.x < b.position.x;
	}

	bool compareY(const Apple& a, const Apple& b) {
		return a.position.y < b.position.y;
	}

	KDNode::NodePtr build2DTree(Apple* apples, int start, int end, bool splitByX) {
		if (start > end) {
			return nullptr;
		}

		int mid = (start + end) / 2;

		if (splitByX) {
			std::nth_element(apples + start, apples + mid, apples + end + 1, [](const Apple& a, const Apple& b) {
				return a.position.x < b.position.x;
				});
		}
		else {
			std::nth_element(apples + start, apples + mid, apples + end + 1, [](const Apple& a, const Apple& b) {
				return a.position.y < b.position.y;
				});
		}

		KDNode::NodePtr node = std::make_shared<KDNode>(apples[mid], build2DTree(apples, start, mid - 1, !splitByX), build2DTree(apples, mid + 1, end, !splitByX));

		return node;
	}

	double squaredDistance(const Position& a, const Position& b) {
		return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
	}

	void nearestNeighbor(const std::shared_ptr<KDNode>& root, const Position& target, bool splitByX, std::shared_ptr<KDNode>& best, double& bestDist) {
		if (!root) {
			return;
		}

		double dist = squaredDistance(target, root->apple->position);
		if (dist < bestDist) {
			bestDist = dist;
			best = root;
		}

		double diff = (splitByX ? target.x - root->apple->position.x : target.y - root->apple->position.y);

		std::shared_ptr<KDNode> near = (diff < 0) ? root->left : root->right;
		std::shared_ptr<KDNode> far = (diff < 0) ? root->right : root->left;

		if (near) {
			nearestNeighbor(near, target, !splitByX, best, bestDist);
		}

		if (far && diff * diff < bestDist) {
			nearestNeighbor(far, target, !splitByX, best, bestDist);
		}
	}

	NodePtr findNearestNeighbor(const std::shared_ptr<KDNode>& root, const Position& target) {
		std::shared_ptr<KDNode> best = nullptr;
		double bestDist = std::numeric_limits<double>::max();
		nearestNeighbor(root, target, true, best, bestDist);
		return best;
	}

	NodePtr insertPoint(NodePtr root, Apple point, bool splitByX) {
		if (root == nullptr) {
			return std::make_shared<KDNode>();
		}

		if ((splitByX && point.position.x < root->apple->position.x) || (!splitByX && point.position.y < root->apple->position.y)) {
			root->left = insertPoint(root->left, point, !splitByX);
		}
		else {
			root->right = insertPoint(root->right, point, !splitByX);
		}

		return root;
	}

	KDNode::KDNode(Apple& a, NodePtr l, NodePtr r) :
		apple(std::make_shared<Apple>(a)),
		left(l),
		right(r) {}
}