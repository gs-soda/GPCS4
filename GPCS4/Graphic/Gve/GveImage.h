#pragma once

#include "GveCommon.h"
#include "GveMemory.h"
#include "GveDescriptor.h"
#include "GveGpuResource.h"

namespace gve
{;

class GveDevice;

struct GveImageCreateInfo
{
	/// Image dimension
	VkImageType type;

	/// Pixel format
	VkFormat format;

	/// Flags
	VkImageCreateFlags flags;

	/// Sample count for MSAA
	VkSampleCountFlagBits sampleCount;

	/// Image size, in texels
	VkExtent3D extent;

	/// Number of image array layers
	uint32_t numLayers;

	/// Number of mip levels
	uint32_t mipLevels;

	/// Image usage flags
	VkImageUsageFlags usage;

	/// Pipeline stages that can access
	/// the contents of the image
	VkPipelineStageFlags stages;

	/// Allowed access pattern
	VkAccessFlags access;

	/// Image tiling mode
	VkImageTiling tiling;

	/// Common image layout
	VkImageLayout layout;

	// Initial image layout
	VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
};

struct GveImageViewCreateInfo
{
	/// Image view dimension
	VkImageViewType type = VK_IMAGE_VIEW_TYPE_2D;

	/// Pixel format
	VkFormat format = VK_FORMAT_UNDEFINED;

	/// Image view usage flags
	VkImageUsageFlags usage = 0;

	/// Subresources to use in the view
	VkImageAspectFlags aspect = 0;

	uint32_t minLevel  = 0;
	uint32_t numLevels = 0;
	uint32_t minLayer  = 0;
	uint32_t numLayers = 0;

	/// Component mapping. Defaults to identity.
	VkComponentMapping swizzle = {
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY,
	};
};


class GveImage : public GveGpuResource
{
public:
	GveImage(
		const RcPtr<GveDevice>&    device,
		const GveImageCreateInfo&  createInfo,
		GveMemoryAllocator*        memAlloc,
		VkMemoryPropertyFlags      memFlags);

	// Creates image object from existing image
	// Typically implementation-managed, like swapchain image.
	GveImage(
		const RcPtr<GveDevice>&   device,
		const GveImageCreateInfo& info,
		VkImage                   image);

	~GveImage();

	VkImage handle() const;

	const GveImageCreateInfo& info() const;

	VkImageLayout pickLayout(VkImageLayout target) const;


private:
	RcPtr<GveDevice>   m_device;
	GveImageCreateInfo m_info;

	VkImage   m_image = VK_NULL_HANDLE;
	GveMemory m_memory;
};


///

class GveImageView : public GveGpuResource
{
public:
	GveImageView(const RcPtr<GveDevice>&       device,
				 const GveImageViewCreateInfo& createInfo,
				 const RcPtr<GveImage>&        image);
	~GveImageView();

	VkImageView handle() const;

	RcPtr<GveImage> getImage();

	const GveImageViewCreateInfo& info() const;

	const GveImageCreateInfo& imageInfo() const;

	GveDescriptorInfo getDescriptor(VkImageViewType type, VkImageLayout layout) const;

	VkExtent3D mipLevelExtent(uint32_t mipLevel) const;

private:
	RcPtr<GveDevice>       m_device;
	GveImageViewCreateInfo m_info;
	RcPtr<GveImage>        m_image;

	VkImageView m_imageView;
};


}  // namespace gve