#ifndef MVPMODEL_H
#define MVPMODEL_H
#include "modelData.h"
#include "rendering/Paintbrush.h"
#include <QColor>

/**
 * \brief Handles any data related to the core application (default UI settings, file system access, model data)
 */
class MvpModel
{
public:
	MvpModel();

	// Set the default settings
	void default_settings();

	// File system methods
	bool export_hair_to_disk(const QString& filename);
	bool export_hairstyle_to_disk(const QImage& image, const QString& filename);
	QImage load_hairstyle_from_disk(const QString& filename);
	void load_models();

	// Getters/Setters
	ModelData* get_reference_model() const { return m_reference_model_; }
	ModelData* get_growth_mesh() const { return m_growth_mesh_; }

	// Default UI settings
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
	bool m_light_mesh_default{};
	QColor m_light_color_default;
	// meshes
	bool m_growthmesh_show_default{};
	bool m_referencemodel_show_default{};

private:
	ModelData* m_reference_model_{};
	ModelData* m_growth_mesh_{};
};

#endif // MVPMODEL_H
