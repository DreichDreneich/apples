#pragma once
#include <memory>
#include "Apple.h"

namespace ApplesGame
{
	using ApplePtr = std::shared_ptr<Apple>;

	class KDNode {
	public:
		using NodePtr = std::shared_ptr<KDNode>;

		ApplePtr apple;
		NodePtr left;
		NodePtr right;

		KDNode() = default;
		KDNode(Apple& a, NodePtr l, NodePtr r);
	};

	using NodePtr = std::shared_ptr<KDNode>;

	NodePtr build2DTree(Apple* points, int start, int end, bool splitByX);
	NodePtr findNearestNeighbor(const std::shared_ptr<KDNode>& root, const Position& target);
	std::vector<NodePtr> findPointsWithinRadius(const NodePtr root, const Position& target, double radius);
}