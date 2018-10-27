#ifndef SETTINGS_H
#define SETTINGS_H

#include "rendering/scene_elements/paintbrush.h"
#include <QColor>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

struct UiSettings
{
	bool m_grid_visibility{true};

	// Hair
	double m_hairlength{3.00};
	int m_hairsegment_count{4};
	QColor m_hair_color{255,255,255};
	QColor m_hair_root_color{0,0,0};

	// Brush
	Paintbrush::paintmode m_brushmode{Paintbrush::paintmode::length};
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


struct Project
{
	UiSettings m_ui_settings;
	std::string m_growthmesh_path;
	std::string m_hair_style_path;

};

inline void to_json(json& j, const Project& s) {
    j = json
 {	
     {"version", 0}, 
       	{"version_string","0.01"}, 
		{"hair_settings",{"max_hair_length" , s.m_ui_settings.m_hairlength}}
    };
}

inline void from_json(const json& j, Project& s) {

	// UI settings
	s.m_ui_settings.m_hairlength = j.at("hair_settings").at("max_hair_length").get<double>();
	s.m_ui_settings.m_hairsegment_count = j.at("hair_settings").at("segments_per_hair").get<int>();
	
	auto r = j.at("hair_settings").at("hair_color").at(0).get<int>();
	auto g = j.at("hair_settings").at("hair_color").at(1).get<int>();
	auto b = j.at("hair_settings").at("hair_color").at(2).get<int>();
	s.m_ui_settings.m_hair_color = QColor(r,g,b);
	
	r = j.at("hair_settings").at("root_color").at(0).get<int>();
	g = j.at("hair_settings").at("root_color").at(1).get<int>();
	b = j.at("hair_settings").at("root_color").at(2).get<int>();
	s.m_ui_settings.m_hair_root_color = QColor(r,g,b);

	s.m_ui_settings.m_brushmode = static_cast<Paintbrush::paintmode>( j.at("brush_settings").at("paint mode").get<int>());
	s.m_ui_settings.m_brush_size =  j.at("brush_settings").at("brush_size").get<double>();
	s.m_ui_settings.m_brush_intensity =  j.at("brush_settings").at("brush_intensity").get<double>();

	s.m_ui_settings.m_light_hair = j.at("light_settings").at("light_hair").get<bool>();
	s.m_ui_settings.m_light_mesh = j.at("light_settings").at("light_mesh").get<bool>();
	
	r = j.at("light_settings").at("light_color").at(0).get<int>();
	g = j.at("light_settings").at("light_color").at(1).get<int>();
	b = j.at("light_settings").at("light_color").at(2).get<int>();
	s.m_ui_settings.m_light_color = QColor(r,g,b);

	s.m_ui_settings.m_growthmesh_show = j.at("mesh_settings").at("show_growth_mesh").get<bool>();
	s.m_ui_settings.m_referencemodel_show = j.at("mesh_settings").at("show_reference_model").get<bool>();
	
	// File paths
	s.m_growthmesh_path = j.at("file_settings").at("growth_mesh").get<std::string>();
}
#endif