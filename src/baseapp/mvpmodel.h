#ifndef MVPMODEL_H
#define MVPMODEL_H

#include "modelData.h"

#include "settings.h"
#include "hairData.h"

#include <memory>

#include <QImage>

/**
 * \brief Handles any data related to the core application (default UI settings, file system access, model data)
 */
class MvpModel
{
public:

	Project load_project_file_from_disk(const QString& filename) const;
	void save_project_file_to_disk(const QString& filename, const Project& proj) const;

	// File system methods
	void export_hair_to_disk(const QString& filename, const HairData& hairdata) const;
	bool export_hairstyle_to_disk(const QImage& image, const QString& filename) const;
	QImage load_hairstyle_from_disk(const QString& filename) const;

	void load_fbx_model_from_disk(const QString& filename);
	
	// Getters/Setters
	ModelData* get_fbx_model() const noexcept { return m_fbx_model_.get(); }

private:
	std::unique_ptr<ModelData> m_fbx_model_;
};

#endif // MVPMODEL_H
