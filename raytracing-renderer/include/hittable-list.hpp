#pragma once

#include "hittable.hpp"
#include <memory>
#include <vector>

class HittableList : public Hittable {
public:
  std::vector<std::shared_ptr<Hittable>> objects;

  HittableList();
  HittableList(std::shared_ptr<Hittable> object);

  void clear();
  void add(std::shared_ptr<Hittable> object);

  bool hit(const Ray &r, const Interval& ray_t, HitRecord &rec) const override;
};
