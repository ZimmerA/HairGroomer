#ifndef SETTINGS_H
#define SETTINGS_H

#include "rendering/scene_elements/paintbrush.h"

#include <QColor>

#undef snprintf // BAD FBX SDK!
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct UiSettings
{
	bool m_grid_visibility{true};
	int m_growth_mesh_index{0};
	int m_up_axis_index{0};
	// Hair
	double m_hairlength{3.00};
	int m_hairsegment_count{4};
	QColor m_hair_color{255, 255, 255};
	QColor m_hair_root_color{0, 0, 0};

	// Brush
	PaintBrush::PaintMode m_brushmode{PaintBrush::PaintMode::Length};
	double m_brush_size{0.10};
	double m_brush_intensity{1.0};

	// Light
	bool m_light_hair{true};
	bool m_light_mesh{true};
	QColor m_light_color{255, 255, 255};

	// meshes
	bool m_growthmesh_show{true};
	bool m_referencemodel_show{true};
};


struct ProjectSettings
{
	UiSettings m_ui_settings;
	std::string m_growthmesh_name;
	std::string m_hair_style_name;
};

inline void to_json(json& j, const ProjectSettings& s)
{
	j = json
	{
		{"version", 0},
		{"version_string", "0.01"},
		{
			"general_settings", {
				{"show_uv_grid", s.m_ui_settings.m_grid_visibility},
				{"growth_mesh_index", s.m_ui_settings.m_growth_mesh_index},
				{"up_axis_index", s.m_ui_settings.m_up_axis_index}
			}
		},
		{
			"hair_settings", {
				{"max_hair_length", s.m_ui_settings.m_hairlength},
				{"segments_per_hair", s.m_ui_settings.m_hairsegment_count},
				{
					"hair_color",
					{
						s.m_ui_settings.m_hair_color.red(), s.m_ui_settings.m_hair_color.green(),
						s.m_ui_settings.m_hair_color.blue()
					}
				},
				{
					"root_color",
					{
						s.m_ui_settings.m_hair_root_color.red(), s.m_ui_settings.m_hair_root_color.green(),
						s.m_ui_settings.m_hair_root_color.blue()
					}
				}
			}
		},
		{
			"brush_settings", {
				{"paint_mode", static_cast<int>(s.m_ui_settings.m_brushmode)},
				{"brush_size", s.m_ui_settings.m_brush_size},
				{"brush_intensity", s.m_ui_settings.m_brush_intensity}
			}
		},
		{
			"light_settings", {
				{"light_hair", s.m_ui_settings.m_light_hair},
				{"light_mesh", s.m_ui_settings.m_light_mesh},
				{
					"light_color",
					{
						s.m_ui_settings.m_light_color.red(), s.m_ui_settings.m_light_color.green(),
						s.m_ui_settings.m_light_color.blue()
					}
				}
			}
		},
		{
			"mesh_settings", {
				{"show_reference_model", s.m_ui_settings.m_referencemodel_show},
				{"show_growth_mesh", s.m_ui_settings.m_growthmesh_show}
			}
		},
		{
			"file_settings", {
				{"growth_mesh", s.m_growthmesh_name},
				{"hair_style", s.m_hair_style_name}
			}
		}
	};
}

inline void from_json(const json& j, ProjectSettings& s)
{
	// UI settings
	s.m_ui_settings.m_grid_visibility = j.at("general_settings").at("show_uv_grid").get<bool>();
	s.m_ui_settings.m_growth_mesh_index = j.at("general_settings").at("growth_mesh_index").get<int>();
	s.m_ui_settings.m_up_axis_index = j.at("general_settings").at("up_axis_index").get<int>();
	s.m_ui_settings.m_hairlength = j.at("hair_settings").at("max_hair_length").get<double>();
	s.m_ui_settings.m_hairsegment_count = j.at("hair_settings").at("segments_per_hair").get<int>();

	auto r = j.at("hair_settings").at("hair_color").at(0).get<int>();
	auto g = j.at("hair_settings").at("hair_color").at(1).get<int>();
	auto b = j.at("hair_settings").at("hair_color").at(2).get<int>();
	s.m_ui_settings.m_hair_color = QColor(r, g, b);

	r = j.at("hair_settings").at("root_color").at(0).get<int>();
	g = j.at("hair_settings").at("root_color").at(1).get<int>();
	b = j.at("hair_settings").at("root_color").at(2).get<int>();
	s.m_ui_settings.m_hair_root_color = QColor(r, g, b);

	s.m_ui_settings.m_brushmode = static_cast<PaintBrush::PaintMode>(j.at("brush_settings").at("paint_mode").get<int>()
	);
	s.m_ui_settings.m_brush_size = j.at("brush_settings").at("brush_size").get<double>();
	s.m_ui_settings.m_brush_intensity = j.at("brush_settings").at("brush_intensity").get<double>();

	s.m_ui_settings.m_light_hair = j.at("light_settings").at("light_hair").get<bool>();
	s.m_ui_settings.m_light_mesh = j.at("light_settings").at("light_mesh").get<bool>();

	r = j.at("light_settings").at("light_color").at(0).get<int>();
	g = j.at("light_settings").at("light_color").at(1).get<int>();
	b = j.at("light_settings").at("light_color").at(2).get<int>();
	s.m_ui_settings.m_light_color = QColor(r, g, b);

	s.m_ui_settings.m_growthmesh_show = j.at("mesh_settings").at("show_growth_mesh").get<bool>();
	s.m_ui_settings.m_referencemodel_show = j.at("mesh_settings").at("show_reference_model").get<bool>();

	// File paths
	s.m_growthmesh_name = j.at("file_settings").at("growth_mesh").get<std::string>();
	s.m_hair_style_name = j.at("file_settings").at("hair_style").get<std::string>();
}
#endif
