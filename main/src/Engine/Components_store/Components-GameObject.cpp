#if defined(__GNUC__) //&& !defined(__clang__)
module Components;
import :GameObject;
#else
module Components:GameObject;
import Components;
#if defined(__clang__)
import :Animator;
import :Camera;
import :Light;
import :MeshFilter;
import :Rigidbody;
import :Script;
import :Transform;
#endif
#endif
import LayerMask;
import Scene;
import System;
import Application;
import Common3D;
using namespace System;
using namespace System::Application;
using namespace System::Application::Common3D;
using namespace Engine;

void GameObject::LastUpdate() noexcept {
	for (Script* s : m_scripts) if (s->EnableLastUpdate) s->LastUpdate();
	Animator* animator = GetComponentPtr<Animator>();
	if (animator) animator->Update();
}
void GameObject::FixedUpdate() noexcept {
	for (Script* s : m_scripts) if (s->EnableFixedUpdate) s->FixedUpdate();
	Rigidbody* rigidbody = GetComponentPtr<Rigidbody>();
	if (rigidbody) rigidbody->FixedUpdate();
}


void GameObject::UpdateAllHeap() noexcept {
	UpdateHeap();
	Animator* animator = GetComponentPtr<Animator>();
	if (animator) animator->UpdateHeap();
	Camera* camera = GetComponentPtr<Camera>();
	if (camera) camera->UpdateHeap();
}

bool GameObject::UpdateCommandList() noexcept {
	m_commandList = CommandList();
	MeshFilter* meshFilter = GetComponentPtr<MeshFilter>();
	if (!meshFilter) return false;
	uint32_t materialCount = meshFilter->GetMaterialCount();
	m_commandList = CreateCommandList(String::Empty(), materialCount);
	m_commandList.BeginCommand();
	Animator* animator = GetComponentPtr<Animator>();
	if (animator) m_commandList.SetAnimationHeap(animator->GetHeap());
	else m_commandList.SetAnimationHeap(Common3D::GetHeap(Common3D::DefaultAnimationHeapName));
	m_commandList.SetObjectHeap(m_objectHeap);
	MeshResource meshResource = meshFilter->GetMeshResource();
	m_commandList.SetMesh(meshResource);
	m_commandList.SetMaterialHeap(meshFilter->GetHeap());
	uint32_t startIndex = 0;
	uint32_t materialIndex = 0;
	for (Material& material : meshFilter->GetMaterials()) {
		m_commandList.SetMaterialIndex(materialIndex);
		m_commandList.SetRenderer(material.renderer);
		uint32_t useIndexCount = meshResource.GetIndexCountsInMaterial(materialIndex);
		m_commandList.DrawIndexed(startIndex, useIndexCount);
		startIndex += useIndexCount;
		++materialIndex;
	}
	m_commandList.EndCommand();
	return true;
}

CommandList GameObject::GetCommandList() noexcept {
	if (m_commandList) return m_commandList;
	UpdateCommandList();
	return m_commandList;
}
