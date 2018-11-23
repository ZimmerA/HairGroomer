#ifndef MVPMODEL_H
#define MVPMODEL_H

#include <memory>

#include <QImage>

#include "modelData.h"

#include "projectsettings.h"
#include "hairData.h"

/**
 * \brief Handles any data related to the core application
 */
class MvpModel
{
public:

	ProjectSettings load_project_file_from_disk(const QString &filename) const;
	void save_project_file_to_disk(const QString &filename, const ProjectSettings &proj) const;

	// File system methods
	void export_hair_to_disk(const QString &filename, const HairData &hairdata, int up_axis_index) const;
	bool export_hairstyle_to_disk(const QImage &image, const QString &filename) const;
	QImage load_hairstyle_from_disk(const QString &filename) const;

	void load_fbx_model_from_disk(const QString &filename);
	void reset_fbx_model();

	inline ModelData* get_fbx_model() const noexcept { return m_fbx_model_.get(); }

	std::string m_loaded_project_path;
	std::string m_loaded_hairstyle_path;

private:

	std::unique_ptr<ModelData> m_fbx_model_;
};

#endif // MVPMODEL_H