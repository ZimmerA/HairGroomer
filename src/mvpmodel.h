#ifndef MVPMODEL_H
#define MVPMODEL_H
#include "modelData.h"
#include "opengl/Paintbrush.h"
#include <QColor>

/**
 * \brief Stores default settings and handles anything data related
 */
class MvpModel
{
public:
	MvpModel();
	void default_settings();
	void load_models();
	
	bool export_hair_to_disk(const QString& filename);
	bool export_hairstyle_to_disk(const QImage& image, const QString& filename);
	QImage load_hairstyle_from_disk(const QString& filename);
	
	// Getters/Setters
	ModelData* get_reference_model() const { return m_reference_model_; }
	ModelData* get_growth_mesh() const { return m_growth_mesh_; }

	// Default settings
	// General
	bool m_grid_visibility_default{};
	// Hair
	double m_hairlength_default{};
	double m_hairsegment_count_default{};
	QColor m_hair_color_default;
	QColor m_hair_root_color_default;

	// Brush
	Paintbrush::paintmode m_brushmode_default{};
	double m_brush_size_default{};
	double m_brush_intensity_default{};
	// Light
	bool m_light_hair_default{};
	QColor m_light_color_default;

private:
	ModelData* m_reference_model_{};
	ModelData* m_growth_mesh_{};
};

#endif // MVPMODEL_H
