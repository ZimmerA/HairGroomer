#ifndef MVPMODEL_H
#define MVPMODEL_H
#include <modelData.h>

class MvpModel
{
public:
	MvpModel();
	void load_models();

	// Getters/Setters
	ModelData* get_reference_model() const { return m_reference_model_; }
	ModelData* get_growth_mesh() const { return m_growth_mesh_; }

private:
	ModelData* m_reference_model_{};
	ModelData* m_growth_mesh_{};
};

#endif // MVPMODEL_H
