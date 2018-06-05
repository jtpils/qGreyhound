#include "PDALConverter.h"

#include <array>

#include <ccScalarField.h>
#include <ccColorScalesManager.h>

bool 
is_vector_zero(const CCVector3d& vec)
{
	return (vec - CCVector3d(0, 0, 0)).norm() < std::numeric_limits<double>::epsilon();
}

void
PDALConverter::convert(pdal::PointViewPtr view, pdal::PointLayoutPtr layout, ccPointCloud *cloud)
{
	using DimId = pdal::Dimension::Id;

	if (!cloud || !cloud->reserve(view->size())) {
		return;
	}
	
	if (layout->hasDim(DimId::X) || layout->hasDim(DimId::Y) || layout->hasDim(DimId::Z)) {
		if (is_vector_zero(m_shift)) {
			pdal::BOX3D bounds;
			view->calculateBounds(bounds);
			m_shift = CCVector3d(bounds.minx, bounds.miny, bounds.minz);
		}

		for (size_t i = 0; i < view->size(); ++i) {
			cloud->addPoint(CCVector3(
				view->getFieldAs<double>(DimId::X, i) - m_shift.x,
				view->getFieldAs<double>(DimId::Y, i) - m_shift.y,
				view->getFieldAs<double>(DimId::Z, i) - m_shift.z
			));
		}
		cloud->setGlobalShift(m_shift);
	}

	pdal::Dimension::IdList color_ids{ DimId::Red, DimId::Green, DimId::Blue };
	bool has_all_colors = true;
	for (auto color_id : color_ids) {
		if (!view->hasDim(color_id)) {
			has_all_colors = false;
			break;
		}
	}

	if (has_all_colors) {
		convert_rgb(view, cloud);
	}
	convert_scalar_fields(view, layout, cloud);
}

void 
PDALConverter::convert_rgb(pdal::PointViewPtr view, ccPointCloud *out_cloud)
{
	if (!out_cloud->size()) {
		return;
	}
	if (!out_cloud->reserveTheRGBTable()) {
		ccLog::Error("Failed to allocate memory for the colors.");
	}
	else
	{
		std::array<uint16_t, 3> rgb_16 { 0,0,0 };
		std::array<ColorCompType, 3> rgb { 0,0,0 };
		for (size_t i = 0; i < view->size(); ++i) {
			rgb_16[0] = view->getFieldAs<uint16_t>(pdal::Dimension::Id::Red, i);
			rgb_16[1] = view->getFieldAs<uint16_t>(pdal::Dimension::Id::Green, i);
			rgb_16[2] = view->getFieldAs<uint16_t>(pdal::Dimension::Id::Blue, i);

			//TODO: Some resources may not have colors coded on the 16 bits
			// thus not requiring the shift
			rgb[0] = static_cast<ColorCompType>(rgb_16[0] >> 8);
			rgb[1] = static_cast<ColorCompType>(rgb_16[1] >> 8);
			rgb[2] = static_cast<ColorCompType>(rgb_16[2] >> 8);
			out_cloud->addRGBColor(rgb.data());
		}
		out_cloud->showColors(true);
	}
}

void 
PDALConverter::convert_scalar_fields(pdal::PointViewPtr view, pdal::PointLayoutPtr layout, ccPointCloud* out_cloud)
{
	for (pdal::Dimension::Id id : view->dims()) {
		if (id == pdal::Dimension::Id::X ||
			id == pdal::Dimension::Id::Y ||
			id == pdal::Dimension::Id::Z) {
			continue;
		}
		ccScalarField *sf = new ccScalarField(layout->dimName(id).c_str());
		sf->reserve(view->size());

		for (size_t i = 0; i < view->size(); ++i) {
			ScalarType value = view->getFieldAs<ScalarType>(id, i);
			sf->addElement(value);

		}

		sf->computeMinAndMax();

		int sf_index = out_cloud->addScalarField(sf);
		if (id == pdal::Dimension::Id::Intensity) {
			sf->setColorScale(ccColorScalesManager::GetDefaultScale(ccColorScalesManager::GREY));
			out_cloud->setCurrentDisplayedScalarField(sf_index);
			out_cloud->showSF(true);
		}

	}
}

void PDALConverter::set_shift(CCVector3d shift)
{
	m_shift = std::move(shift);
}