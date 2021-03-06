///////////////////////////////////////////////////////////////////////////////
/*
 *	PEEL - Physics Engine Evaluation Lab
 *	Copyright (C) 2012 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/blog.htm
 */
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Render.h"
#include "TestScenes.h"
#include "TestScenesHelpers.h"
#include "PintObjectsManager.h"
#include "MyConvex.h"
#include "GUI_Helpers.h"

///////////////////////////////////////////////////////////////////////////////

static const char* gDesc_DynamicBoxVsLargeStaticSphere = "A dynamic box vs a large static sphere.";

START_TEST(DynamicBoxVsLargeStaticSphere, CATEGORY_CONTACT_GENERATION, gDesc_DynamicBoxVsLargeStaticSphere)

	virtual	void	GetSceneParams(PINT_WORLD_CREATE& desc)
	{
		TestBase::GetSceneParams(desc);
		desc.mCamera[0] = CameraPose(Point(15.53f, 12.54f, 14.78f), Point(-0.67f, -0.37f, -0.65f));
	}

	virtual bool	Setup(Pint& pint, const PintCaps& caps)
	{
		if(!caps.mSupportRigidBodySimulation)
			return false;

		const float Radius = 10.0f;

		{
			PINT_BOX_CREATE Create(1.0f, 1.0f, 1.0f);
			Create.mRenderer	= CreateBoxRenderer(Create.mExtents);
			CreateDynamicObject(pint, &Create, Point(0.0f, Radius + 4.0f, 0.0f));
		}

		{
			PINT_SPHERE_CREATE Create(Radius);
			Create.mRenderer	= CreateSphereRenderer(Create.mRadius);
			CreateStaticObject(pint, &Create, Point(0.0f, 0.0f, 0.0f));
		}
		return true;
	}

END_TEST(DynamicBoxVsLargeStaticSphere)

///////////////////////////////////////////////////////////////////////////////

static const char* gDesc_DynamicConvexVsVeryLargeStaticSphere = "A dynamic convex vs a very large static sphere. The sphere is so large that it looks more like a plane. \
Most engines are not prepared for such a case and objects jitter on top of the sphere...";

START_TEST(DynamicConvexVsVeryLargeStaticSphere, CATEGORY_CONTACT_GENERATION, gDesc_DynamicConvexVsVeryLargeStaticSphere)

	virtual	void	GetSceneParams(PINT_WORLD_CREATE& desc)
	{
		TestBase::GetSceneParams(desc);
		desc.mCamera[0] = CameraPose(Point(9.38f, 23.17f, 5.67f), Point(-0.80f, -0.14f, -0.58f));
	}

	virtual bool	Setup(Pint& pint, const PintCaps& caps)
	{
		if(!caps.mSupportRigidBodySimulation || !caps.mSupportConvexes)
			return false;

		{
			PINT_SPHERE_CREATE Create(10000.0f);
			Create.mRenderer	= CreateSphereRenderer(Create.mRadius);
			CreateStaticObject(pint, &Create, Point(0.0f, -10000.0f+20.0f, 0.0f));
		}

		{
			Point Pts[16];
			udword NbPts = GenerateConvex(Pts, 8, 8, 2.0f, 3.0f, 2.0f);
			ASSERT(NbPts==16);

			PINT_CONVEX_CREATE ConvexCreate(16, Pts);
			ConvexCreate.mRenderer	= CreateConvexRenderer(16, Pts);

			PintObjectHandle Handle = CreateDynamicObject(pint, &ConvexCreate, Point(0.0f, 30.0f, 0.0f));
			ASSERT(Handle);
		}
		mCreateDefaultEnvironment = false;
		return true;
	}

END_TEST(DynamicConvexVsVeryLargeStaticSphere)

///////////////////////////////////////////////////////////////////////////////

static const char* gDesc_SphereOnLargeBox = "A sphere on a large box.";

START_TEST(SphereOnLargeBox, CATEGORY_CONTACT_GENERATION, gDesc_SphereOnLargeBox)

	virtual	void	GetSceneParams(PINT_WORLD_CREATE& desc)
	{
		TestBase::GetSceneParams(desc);
		desc.mCamera[0] = CameraPose(Point(-35.08f, 9.73f, 32.02f), Point(0.67f, -0.13f, -0.73f));
	}

	virtual bool	Setup(Pint& pint, const PintCaps& caps)
	{
		if(!caps.mSupportRigidBodySimulation)
			return false;

		const float Radius = 1.0f;

		{
			PINT_BOX_CREATE Create(20.0f, 2.0f, 20.0f);
			Create.mRenderer	= CreateBoxRenderer(Create.mExtents);
			CreateDynamicObject(pint, &Create, Point(0.0f, 2.0f, 0.0f));
		}

		{
			PINT_SPHERE_CREATE Create(Radius);
			Create.mRenderer	= CreateSphereRenderer(Create.mRadius);
			CreateDynamicObject(pint, &Create, Point(0.0f, 8.0f, 0.0f));
		}
		return true;
	}

END_TEST(SphereOnLargeBox)

///////////////////////////////////////////////////////////////////////////////

static const char* gDesc_BoxOnLargeBox = "A box on a large box.";

START_TEST(BoxOnLargeBox, CATEGORY_CONTACT_GENERATION, gDesc_BoxOnLargeBox)

	virtual	void	GetSceneParams(PINT_WORLD_CREATE& desc)
	{
		TestBase::GetSceneParams(desc);
		desc.mCamera[0] = CameraPose(Point(-29.88f, 7.64f, 30.09f), Point(0.65f, -0.06f, -0.76f));
	}

	virtual bool	Setup(Pint& pint, const PintCaps& caps)
	{
		if(!caps.mSupportRigidBodySimulation)
			return false;

		{
			PINT_BOX_CREATE Create(20.0f, 2.0f, 20.0f);
			Create.mRenderer	= CreateBoxRenderer(Create.mExtents);
			CreateDynamicObject(pint, &Create, Point(0.0f, 2.0f, 0.0f));
		}

		{
			PINT_BOX_CREATE Create(4.0f, 1.0f, 1.0f);
			Create.mRenderer	= CreateBoxRenderer(Create.mExtents);
			CreateDynamicObject(pint, &Create, Point(0.0f, 8.0f, 0.0f));
		}
		return true;
	}

END_TEST(BoxOnLargeBox)

///////////////////////////////////////////////////////////////////////////////

static const char* gDesc_CapsuleOnLargeBox = "A capsule on a large box. Should generate two VF contacts.";

START_TEST(CapsuleOnLargeBox, CATEGORY_CONTACT_GENERATION, gDesc_CapsuleOnLargeBox)

	virtual	void	GetSceneParams(PINT_WORLD_CREATE& desc)
	{
		TestBase::GetSceneParams(desc);
		desc.mCamera[0] = CameraPose(Point(-29.88f, 7.64f, 30.09f), Point(0.65f, -0.06f, -0.76f));
	}

	virtual bool	Setup(Pint& pint, const PintCaps& caps)
	{
		if(!caps.mSupportRigidBodySimulation)
			return false;

		const float Radius = 1.0f;
		const float HalfHeight = 4.0f;

		{
			PINT_BOX_CREATE Create(20.0f, 2.0f, 20.0f);
			Create.mRenderer	= CreateBoxRenderer(Create.mExtents);
			CreateDynamicObject(pint, &Create, Point(0.0f, 2.0f, 0.0f));
		}

		{
			PINT_CAPSULE_CREATE Create(Radius, HalfHeight);
			Create.mRenderer	= CreateCapsuleRenderer(Create.mRadius, Create.mHalfHeight*2.0f);

			const Quat q = ShortestRotation(Point(0.0f, 1.0f, 0.0f), Point(1.0f, 0.0f, 0.0f));
			CreateDynamicObject(pint, &Create, Point(0.0f, 8.0f, 0.0f), &q);
		}
		return true;
	}

END_TEST(CapsuleOnLargeBox)

///////////////////////////////////////////////////////////////////////////////

static const char* gDesc_ThinCapsuleOnBox = "Very thin capsule on static box. This is a stress test for the capsule-box contact generation.";

START_TEST(ThinCapsuleOnBox, CATEGORY_CONTACT_GENERATION, gDesc_ThinCapsuleOnBox)

	virtual	void	GetSceneParams(PINT_WORLD_CREATE& desc)
	{
		TestBase::GetSceneParams(desc);
		desc.mCamera[0] = CameraPose(Point(-4.55f, 6.41f, 2.30f), Point(0.68f, -0.55f, -0.48f));
	}

	virtual bool	Setup(Pint& pint, const PintCaps& caps)
	{
		if(!caps.mSupportRigidBodySimulation)
			return false;

		const float Radius = 0.01f;
		const float HalfHeight = 4.0f;

		{
			PINT_BOX_CREATE Create(2.0f, 2.0f, 2.0f);
			Create.mRenderer	= CreateBoxRenderer(Create.mExtents);
			CreateStaticObject(pint, &Create, Point(0.0f, 2.0f, 0.0f));
		}

		{
			PINT_CAPSULE_CREATE Create(Radius, HalfHeight);
			Create.mRenderer	= CreateCapsuleRenderer(Create.mRadius, Create.mHalfHeight*2.0f);

			const Quat q = ShortestRotation(Point(0.0f, 1.0f, 0.0f), Point(1.0f, 0.0f, 0.0f));
			CreateDynamicObject(pint, &Create, Point(0.0f, 4.0f-Radius, 0.0f), &q);
		}
		return true;
	}

END_TEST(ThinCapsuleOnBox)

///////////////////////////////////////////////////////////////////////////////

static const char* gDesc_ThinCapsuleOnBoxes = "Very thin capsule on static boxes. This is a stress test for the capsule-box contact generation.";

START_TEST(ThinCapsuleOnBoxes, CATEGORY_CONTACT_GENERATION, gDesc_ThinCapsuleOnBoxes)

	virtual	void	GetSceneParams(PINT_WORLD_CREATE& desc)
	{
		TestBase::GetSceneParams(desc);
		desc.mCamera[0] = CameraPose(Point(-5.05f, 5.46f, 3.09f), Point(0.74f, -0.22f, -0.64f));
	}

	virtual bool	Setup(Pint& pint, const PintCaps& caps)
	{
		if(!caps.mSupportRigidBodySimulation)
			return false;

		const float Radius = 0.01f;
		const float HalfHeight = 4.0f;

		{
			PINT_BOX_CREATE Create(2.0f, 2.0f, 2.0f);
			Create.mRenderer	= CreateBoxRenderer(Create.mExtents);
			CreateStaticObject(pint, &Create, Point(-5.5f, 2.0f, 0.0f));
		}

		{
			PINT_BOX_CREATE Create(2.0f, 2.0f, 2.0f);
			Create.mRenderer	= CreateBoxRenderer(Create.mExtents);
			CreateStaticObject(pint, &Create, Point(5.5f, 2.0f, 0.0f));
		}

		{
			PINT_CAPSULE_CREATE Create(Radius, HalfHeight);
			Create.mRenderer	= CreateCapsuleRenderer(Create.mRadius, Create.mHalfHeight*2.0f);

			const Quat q = ShortestRotation(Point(0.0f, 1.0f, 0.0f), Point(1.0f, 0.0f, 0.0f));
			CreateDynamicObject(pint, &Create, Point(0.0f, 4.0f-Radius, 0.0f), &q);
		}
		return true;
	}

END_TEST(ThinCapsuleOnBoxes)

///////////////////////////////////////////////////////////////////////////////

static const char* gDesc_PCM_Stress = "Stress test for the PCM algorithm. Engines using PCM have a hard time simulating that one (at least with CCD disabled). \
Ideally the objects would fall to the ground and simply stop. PCM failures produce random weird looking motions, or even objects going through the ground.";

START_TEST(PCM_Stress, CATEGORY_CONTACT_GENERATION, gDesc_PCM_Stress)

	virtual	void	GetSceneParams(PINT_WORLD_CREATE& desc)
	{
		TestBase::GetSceneParams(desc);
		desc.mCamera[0] = CameraPose(Point(33.36f, 23.93f, 38.67f), Point(-0.67f, -0.08f, -0.73f));
	}

	virtual bool	Setup(Pint& pint, const PintCaps& caps)
	{
		if(!caps.mSupportConvexes || !caps.mSupportRigidBodySimulation)
			return false;

		const float Altitude = 10.0f;

		PINT_BOX_CREATE BoxDesc(100.0f, 0.05f, 100.0f);
		BoxDesc.mRenderer	= CreateBoxRenderer(BoxDesc.mExtents);

		PINT_OBJECT_CREATE ObjectDesc;
		ObjectDesc.mShapes		= &BoxDesc;
		ObjectDesc.mPosition.x	= 0.0f;
		ObjectDesc.mPosition.y	= Altitude;
		ObjectDesc.mPosition.z	= 0.0f;
		ObjectDesc.mMass		= 0.0f;
		CreatePintObject(pint, ObjectDesc);

		MyConvex C;
	//	udword i=2;
		udword i=4;
	//	udword i=7;
	//	udword i=13;
		C.LoadFile(i);

		PINT_CONVEX_CREATE ConvexCreate(C.mNbVerts, C.mVerts);
		ConvexCreate.mRenderer	= CreateConvexRenderer(ConvexCreate.mNbVerts, ConvexCreate.mVerts);

		const float Amplitude = 1.5f;
		const udword NbX = 12;
		const udword NbY = 12;

		const float Scale = 4.0f;
		for(udword y=0;y<NbY;y++)
		{
			for(udword x=0;x<NbX;x++)
			{
				const float xf = (float(x)-float(NbX)*0.5f)*Scale;
				const float yf = (float(y)-float(NbY)*0.5f)*Scale;

	//			const Point pos = Point(xf, Altitude + 20.0f, yf);
				const Point pos = Point(xf, Altitude + 30.0f, yf);

				PintObjectHandle Handle = CreateDynamicObject(pint, &ConvexCreate, pos);
				ASSERT(Handle);
			}
		}
		return true;
	}

END_TEST(PCM_Stress)

///////////////////////////////////////////////////////////////////////////////

static const char* gDesc_BoxSlidingOnPlanarMesh = "INTERNAL EDGES: a box sliding on a tessellated planar mesh. This is to test how each engine handles collisions with 'internal edges'. \
Ideally the box would slide smoothly until the end, in a straight trajectory. Typical failures result in the box bumping away from the mesh. There is no friction. The object eventually stops \
because of linear damping - implemented slightly differently in each engine.";

START_TEST(BoxSlidingOnPlanarMesh, CATEGORY_CONTACT_GENERATION, gDesc_BoxSlidingOnPlanarMesh)

	virtual	void	GetSceneParams(PINT_WORLD_CREATE& desc)
	{
		TestBase::GetSceneParams(desc);
		desc.mCamera[0] = CameraPose(Point(17.83f, 24.29f, 24.74f), Point(-0.54f, -0.21f, -0.81f));
	}

	virtual bool	CommonSetup()
	{
		IndexedSurface* IS = CreateManagedSurface();
	//	bool status = IS->MakePlane(32, 32);
		bool status = IS->MakePlane(16, 32);
		ASSERT(status);
		IS->Scale(Point(0.01f, 1.0f, 0.1f));
		IS->Flip();

		return TestBase::CommonSetup();
	}

	virtual bool	Setup(Pint& pint, const PintCaps& caps)
	{
		if(!caps.mSupportMeshes || !caps.mSupportRigidBodySimulation)
			return false;

		const float Altitude = 1.0f;

		const IndexedSurface* IS = GetFirstSurface();

		PINT_MATERIAL_CREATE MatDesc;
		MatDesc.mStaticFriction		= 0.0f;
		MatDesc.mDynamicFriction	= 0.0f;
		MatDesc.mRestitution		= 0.0f;

		PINT_MESH_CREATE MeshDesc;
		MeshDesc.mSurface	= IS->GetSurfaceInterface();
		MeshDesc.mRenderer	= CreateMeshRenderer(MeshDesc.mSurface);
		MeshDesc.mMaterial	= &MatDesc;

		Matrix3x3 m;
		m.RotX(degToRad(42.0f));

		PINT_OBJECT_CREATE ObjectDesc;
		ObjectDesc.mShapes		= &MeshDesc;
		ObjectDesc.mPosition	= Point(0.0f, Altitude, 0.0f);
		ObjectDesc.mRotation	= m;
		ObjectDesc.mMass		= 0.0f;
		CreatePintObject(pint, ObjectDesc);

		const float BoxExtent = 1.0f;
		PintObjectHandle CubeHandle = CreateDynamicBox(pint, 1.0f, 1.0f, 1.0f, Point(0.0f, 40.0f, -41.0f), &ObjectDesc.mRotation, &MatDesc);
		ASSERT(CubeHandle);
		return true;
	}

END_TEST(BoxSlidingOnPlanarMesh)

///////////////////////////////////////////////////////////////////////////////

static IndexedSurface* CreateCurvedMesh(TestBase& test, udword nb_x, udword nb_y, const Point& scale)
{
	IndexedSurface* IS = test.CreateManagedSurface();
	bool status = IS->MakePlane(nb_x, nb_y);
	ASSERT(status);
	IS->Scale(scale);
	IS->Flip();
	Point* P = IS->GetVerts();
	float Offset = 0.1f;
	udword i=0;
	for(udword y=0;y<nb_y;y++)
	{
		for(udword x=0;x<nb_x;x++)
		{
			P[i++].y += Offset;
		}
		Offset *= 1.25f;
	}
	return IS;
}

///////////////////////////////////////////////////////////////////////////////

static const char* gDesc_InternalEdges = "(Configurable test) - An object sliding on a tessellated curved mesh. This is to test how each engine handles collisions with 'internal edges'. \
There is no friction for the object and for the curved mesh (but there is for the ground plane). Ideally the object would slide smoothly until the end, \
in a straight trajectory. Typical failures result in the object bumping away from the mesh.";

#define NB_PRESETS	16
static const udword gPreset_NbX[NB_PRESETS] = {
	4, 4, 4, 16,
	4, 4, 4, 16,
	4, 4, 4, 16,
	4, 4, 4, 16
};
static const float gPreset_ScaleX[NB_PRESETS] = {
	0.1f, 0.1f, 0.01f, 0.01f,
	0.1f, 0.1f, 0.01f, 0.01f,
	0.1f, 0.1f, 0.01f, 0.01f,
	0.1f, 0.1f, 0.01f, 0.01f
};
static const PintShape gPreset_Shape[NB_PRESETS] = {
	PINT_SHAPE_BOX, PINT_SHAPE_BOX, PINT_SHAPE_BOX, PINT_SHAPE_BOX,
	PINT_SHAPE_SPHERE, PINT_SHAPE_SPHERE, PINT_SHAPE_SPHERE, PINT_SHAPE_SPHERE,
	PINT_SHAPE_CAPSULE, PINT_SHAPE_CAPSULE, PINT_SHAPE_CAPSULE, PINT_SHAPE_CAPSULE,
	PINT_SHAPE_CAPSULE, PINT_SHAPE_CAPSULE, PINT_SHAPE_CAPSULE, PINT_SHAPE_CAPSULE
};
static const float gPreset_PosZ[NB_PRESETS] = {
	-55.0f, -54.0f, -55.0f, -54.0f,
	-55.0f, -54.0f, -55.0f, -54.0f,
	-55.0f, -54.0f, -55.0f, -54.0f,
	-55.0f, -54.0f, -55.0f, -54.0f
};
static const bool gPreset_RotateShape[NB_PRESETS] = {
	false, false, false, false,
	false, false, false, false,
	false, false, false, false,
	true, true, true, true
};

class InternalEdges : public TestBase
{
			IndexedSurface*		mSurface;
			PintShapeRenderer*	mRenderer;
			IceEditBox*			mEditBox_NbX;
			IceEditBox*			mEditBox_NbY;
			IceEditBox*			mEditBox_ScaleX;
			IceEditBox*			mEditBox_ScaleZ;
			IceEditBox*			mEditBox_ShapePosX;
			IceEditBox*			mEditBox_ShapePosY;
			IceEditBox*			mEditBox_ShapePosZ;
			IceCheckBox*		mCheckBox_RotateShape;
			IceComboBox*		mComboBox_ShapeType;
			IceComboBox*		mComboBox_Preset;
	public:
							InternalEdges()	:
								mSurface				(null),
								mRenderer				(null),
								mEditBox_NbX			(null),
								mEditBox_NbY			(null),
								mEditBox_ScaleX			(null),
								mEditBox_ScaleZ			(null),
								mEditBox_ShapePosX		(null),
								mEditBox_ShapePosY		(null),
								mEditBox_ShapePosZ		(null),
								mCheckBox_RotateShape	(null),
								mComboBox_ShapeType		(null),
								mComboBox_Preset		(null)
														{										}
	virtual					~InternalEdges()			{										}
	virtual	const char*		GetName()			const	{ return "InternalEdges";				}
	virtual	const char*		GetDescription()	const	{ return gDesc_InternalEdges;			}
	virtual	TestCategory	GetCategory()		const	{ return CATEGORY_CONTACT_GENERATION;	}

	virtual	void			InitUI(PintGUIHelper& helper)
	{
		WindowDesc WD;
		WD.mParent	= null;
		WD.mX		= 50;
		WD.mY		= 50;
		WD.mWidth	= 300;
		WD.mHeight	= 300;
		WD.mLabel	= "InternalEdges";
		WD.mType	= WINDOW_DIALOG;
		IceWindow* UI = ICE_NEW(IceWindow)(WD);
		RegisterUIElement(UI);
		UI->SetVisible(true);

		Container* UIElems = GetUIElements();

		const sdword EditBoxWidth = 60;
		const sdword LabelWidth = 80;
		const sdword OffsetX = LabelWidth + 10;
		const sdword LabelOffsetY = 2;
		const sdword YStep = 20;
		sdword y = 0;
		const bool Enabled = false;
		{
			helper.CreateLabel(UI, 4, y+LabelOffsetY, LabelWidth, 20, "NbX:", UIElems);
			mEditBox_NbX = helper.CreateEditBox(UI, 1, 4+OffsetX, y, EditBoxWidth, 20, "4", UIElems, EDITBOX_INTEGER_POSITIVE, null, null);
			mEditBox_NbX->SetEnabled(Enabled);
			y += YStep;

			helper.CreateLabel(UI, 4, y+LabelOffsetY, LabelWidth, 20, "NbY:", UIElems);
			mEditBox_NbY = helper.CreateEditBox(UI, 1, 4+OffsetX, y, EditBoxWidth, 20, "32", UIElems, EDITBOX_INTEGER_POSITIVE, null, null);
			mEditBox_NbY->SetEnabled(Enabled);
			y += YStep;

			helper.CreateLabel(UI, 4, y+LabelOffsetY, LabelWidth, 20, "ScaleX:", UIElems);
			mEditBox_ScaleX = helper.CreateEditBox(UI, 1, 4+OffsetX, y, EditBoxWidth, 20, "0.1", UIElems, EDITBOX_FLOAT_POSITIVE, null, null);
			mEditBox_ScaleX->SetEnabled(Enabled);
			y += YStep;

			helper.CreateLabel(UI, 4, y+LabelOffsetY, LabelWidth, 20, "ScaleZ:", UIElems);
			mEditBox_ScaleZ = helper.CreateEditBox(UI, 1, 4+OffsetX, y, EditBoxWidth, 20, "0.1", UIElems, EDITBOX_FLOAT_POSITIVE, null, null);
			mEditBox_ScaleZ->SetEnabled(Enabled);
			y += YStep;

			y += YStep;

			helper.CreateLabel(UI, 4, y+LabelOffsetY, LabelWidth, 20, "Shape:", UIElems);
			mComboBox_ShapeType = CreateShapeTypeComboBox(UI, 4+OffsetX, y, Enabled, SSM_UNDEFINED|SSM_SPHERE|SSM_CAPSULE|SSM_BOX);
			mComboBox_ShapeType->Select(PINT_SHAPE_BOX);
			RegisterUIElement(mComboBox_ShapeType);
			y += YStep;

			helper.CreateLabel(UI, 4, y+LabelOffsetY, LabelWidth, 20, "Shape pos x:", UIElems);
			mEditBox_ShapePosX = helper.CreateEditBox(UI, 1, 4+OffsetX, y, EditBoxWidth, 20, "0.0", UIElems, EDITBOX_FLOAT, null, null);
			mEditBox_ShapePosX->SetEnabled(Enabled);
			y += YStep;

			helper.CreateLabel(UI, 4, y+LabelOffsetY, LabelWidth, 20, "Shape pos y:", UIElems);
			mEditBox_ShapePosY = helper.CreateEditBox(UI, 1, 4+OffsetX, y, EditBoxWidth, 20, "80.0", UIElems, EDITBOX_FLOAT, null, null);
			mEditBox_ShapePosY->SetEnabled(Enabled);
			y += YStep;

			helper.CreateLabel(UI, 4, y+LabelOffsetY, LabelWidth, 20, "Shape pos z:", UIElems);
			mEditBox_ShapePosZ = helper.CreateEditBox(UI, 1, 4+OffsetX, y, EditBoxWidth, 20, "-55.0", UIElems, EDITBOX_FLOAT, null, null);
			mEditBox_ShapePosZ->SetEnabled(Enabled);
			y += YStep;

			mCheckBox_RotateShape = helper.CreateCheckBox(UI, 0, 4, y, 400, 20, "Rotate shape", UIElems, false, null, null);
			mCheckBox_RotateShape->SetEnabled(Enabled);
			y += YStep;
		}
		{
			helper.CreateLabel(UI, 4, y+LabelOffsetY, LabelWidth, 20, "Presets:", UIElems);

			class MyComboBox : public IceComboBox
			{
				InternalEdges&	mTest;
				public:
								MyComboBox(const ComboBoxDesc& desc, InternalEdges& test) :
									IceComboBox(desc),
									mTest(test)	{}
				virtual			~MyComboBox()	{}
				virtual	void	OnComboBoxEvent(ComboBoxEvent event)
				{
					if(event==CBE_SELECTION_CHANGED)
					{
						const udword SelectedIndex = GetSelectedIndex();
						const bool Enabled = SelectedIndex==GetItemCount()-1;

						mTest.mEditBox_NbX->SetEnabled(Enabled);
						mTest.mEditBox_NbY->SetEnabled(Enabled);
						mTest.mEditBox_ScaleX->SetEnabled(Enabled);
						mTest.mEditBox_ScaleZ->SetEnabled(Enabled);
						mTest.mEditBox_ShapePosX->SetEnabled(Enabled);
						mTest.mEditBox_ShapePosY->SetEnabled(Enabled);
						mTest.mEditBox_ShapePosZ->SetEnabled(Enabled);
						mTest.mComboBox_ShapeType->SetEnabled(Enabled);
						mTest.mCheckBox_RotateShape->SetEnabled(Enabled);

						if(!Enabled && SelectedIndex<NB_PRESETS)
						{
							mTest.mEditBox_NbX->SetText(_F("%d", gPreset_NbX[SelectedIndex]));
							mTest.mEditBox_NbY->SetText("32");
							mTest.mEditBox_ScaleX->SetText(_F("%.2f", gPreset_ScaleX[SelectedIndex]));
							mTest.mEditBox_ScaleZ->SetText("0.10");
							mTest.mEditBox_ShapePosX->SetText("0.00");
							mTest.mEditBox_ShapePosY->SetText("80.00");
							mTest.mEditBox_ShapePosZ->SetText(_F("%.2f", gPreset_PosZ[SelectedIndex]));
							mTest.mComboBox_ShapeType->Select(gPreset_Shape[SelectedIndex]);
							mTest.mCheckBox_RotateShape->SetChecked(gPreset_RotateShape[SelectedIndex]);
						}
					}
				}
			};

			ComboBoxDesc CBBD;
			CBBD.mID		= 0;
			CBBD.mParent	= UI;
			CBBD.mX			= 4+OffsetX;
			CBBD.mY			= y;
			CBBD.mWidth		= 200;
			CBBD.mHeight	= 20;
			CBBD.mLabel		= "Presets";
			mComboBox_Preset = ICE_NEW(MyComboBox)(CBBD, *this);
			RegisterUIElement(mComboBox_Preset);
			mComboBox_Preset->Add("BoxSlidingOnCurvedMesh");
			mComboBox_Preset->Add("BoxSlidingOnCurvedMesh2");
			mComboBox_Preset->Add("BoxSlidingOnCurvedMesh3");
			mComboBox_Preset->Add("BoxSlidingOnCurvedMesh4");
			mComboBox_Preset->Add("SphereSlidingOnCurvedMesh");
			mComboBox_Preset->Add("SphereSlidingOnCurvedMesh2");
			mComboBox_Preset->Add("SphereSlidingOnCurvedMesh3");
			mComboBox_Preset->Add("SphereSlidingOnCurvedMesh4");
			mComboBox_Preset->Add("CapsuleSlidingOnCurvedMesh");
			mComboBox_Preset->Add("CapsuleSlidingOnCurvedMesh2");
			mComboBox_Preset->Add("CapsuleSlidingOnCurvedMesh3");
			mComboBox_Preset->Add("CapsuleSlidingOnCurvedMesh4");
			mComboBox_Preset->Add("Capsule2SlidingOnCurvedMesh");
			mComboBox_Preset->Add("Capsule2SlidingOnCurvedMesh2");
			mComboBox_Preset->Add("Capsule2SlidingOnCurvedMesh3");
			mComboBox_Preset->Add("Capsule2SlidingOnCurvedMesh4");
			mComboBox_Preset->Add("User-defined");
			mComboBox_Preset->Select(0);
			mComboBox_Preset->SetVisible(true);
			mComboBox_Preset->OnComboBoxEvent(CBE_SELECTION_CHANGED);
			y += YStep;

			y += YStep;
			AddResetButton(UI, 4, y, 300-16);
		}
	}

	virtual	void			GetSceneParams(PINT_WORLD_CREATE& desc)
	{
		TestBase::GetSceneParams(desc);
		desc.mCamera[0] = CameraPose(Point(4.24f, 7.69f, 80.53f), Point(-0.17f, 0.09f, -0.98f));
		desc.mCamera[1] = CameraPose(Point(74.12f, 59.65f, 51.38f), Point(-0.83f, -0.30f, -0.47f));
		desc.mCamera[2] = CameraPose(Point(5.42f, 85.82f, -54.58f), Point(-0.22f, -0.93f, 0.29f));
	}

	virtual bool			CommonSetup()
	{
		const udword NbX = GetFromEditBox(4, mEditBox_NbX);
		const udword NbY = GetFromEditBox(32, mEditBox_NbY);
		const float ScaleX = GetFromEditBox(0.1f, mEditBox_ScaleX, 0.0f, MAX_FLOAT);
		const float ScaleZ = GetFromEditBox(0.1f, mEditBox_ScaleZ, 0.0f, MAX_FLOAT);
//		float ScaleX = 0.1f;
//		float ScaleZ = 0.1f;

/*		printf("%d\n", IR(ScaleX));
		printf("%d\n", IR(ScaleZ));
		float Ref = 0.1f;
		printf("Ref: %d\n", IR(Ref));
*/
/*
1036831948
1036831948
Ref: 1036831949
*/

		mSurface = CreateCurvedMesh(*this, NbX, NbY, Point(ScaleX, 1.0f, ScaleZ));
		mRenderer = CreateMeshRenderer(mSurface->GetSurfaceInterface());

		return TestBase::CommonSetup();
	}

	virtual bool			Setup(Pint& pint, const PintCaps& caps)
	{
		if(!caps.mSupportMeshes || !caps.mSupportRigidBodySimulation)
			return false;

		PINT_MATERIAL_CREATE MatDesc;
		MatDesc.mStaticFriction		= 0.0f;
		MatDesc.mDynamicFriction	= 0.0f;
		MatDesc.mRestitution		= 0.0f;

		// Creates the mesh
		{
			const float Altitude = 1.0f;

			PINT_MESH_CREATE MeshDesc;
			MeshDesc.mSurface	= mSurface->GetSurfaceInterface();
			MeshDesc.mRenderer	= mRenderer;
			MeshDesc.mMaterial	= &MatDesc;

			Matrix3x3 m;
		//	m.RotX(degToRad(42.0f));
			m.RotY(PI);

			PINT_OBJECT_CREATE ObjectDesc;
			ObjectDesc.mShapes		= &MeshDesc;
			ObjectDesc.mPosition	= Point(0.0f, Altitude, 0.0f);
			ObjectDesc.mRotation	= m;
			ObjectDesc.mMass		= 0.0f;
			PintObjectHandle MeshHandle = CreatePintObject(pint, ObjectDesc);
			ASSERT(MeshHandle);
		}

		// Creates the shape
		{
			const PintShape ShapeType = mComboBox_ShapeType ? PintShape(mComboBox_ShapeType->GetSelectedIndex()) : PINT_SHAPE_UNDEFINED;
			const float PosX = GetFromEditBox(0.0f, mEditBox_ShapePosX, MIN_FLOAT, MAX_FLOAT);
			const float PosY = GetFromEditBox(0.0f, mEditBox_ShapePosY, MIN_FLOAT, MAX_FLOAT);
			const float PosZ = GetFromEditBox(0.0f, mEditBox_ShapePosZ, MIN_FLOAT, MAX_FLOAT);
			const Point ShapePos(PosX, PosY, PosZ);

			const bool RotateShape = mCheckBox_RotateShape ? mCheckBox_RotateShape->IsChecked() : false;

			//### doesn't work in Bullet & Havok for some reason
		//	Create.mLocalRot	= ShortestRotation(Point(0.0f, 1.0f, 0.0f), Point(1.0f, 0.0f, 0.0f));
			const Quat q = ShortestRotation(Point(0.0f, 1.0f, 0.0f), Point(1.0f, 0.0f, 0.0f));
			const Quat* Rotation = RotateShape ? &q : null;

			if(ShapeType==PINT_SHAPE_BOX)
			{
				PINT_BOX_CREATE Create(1.0f, 1.0f, 1.0f);
				Create.mRenderer	= CreateBoxRenderer(Create.mExtents);
				Create.mMaterial	= &MatDesc;
				PintObjectHandle ShapeHandle = CreateDynamicObject(pint, &Create, ShapePos, Rotation);
				ASSERT(ShapeHandle);
			}
			else if(ShapeType==PINT_SHAPE_SPHERE)
			{
				PINT_SPHERE_CREATE Create(1.0f);
				Create.mRenderer	= CreateSphereRenderer(Create.mRadius);
				Create.mMaterial	= &MatDesc;
				PintObjectHandle ShapeHandle = CreateDynamicObject(pint, &Create, ShapePos, Rotation);
				ASSERT(ShapeHandle);
			}
			else if(ShapeType==PINT_SHAPE_CAPSULE)
			{
				PINT_CAPSULE_CREATE Create(1.0f, 2.0f);
				Create.mRenderer	= CreateCapsuleRenderer(Create.mRadius, Create.mHalfHeight*2.0f);
				Create.mMaterial	= &MatDesc;
				PintObjectHandle ShapeHandle = CreateDynamicObject(pint, &Create, ShapePos, Rotation);
				ASSERT(ShapeHandle);
			}
		}
		return true;
	}

}InternalEdges;

///////////////////////////////////////////////////////////////////////////////

static const char* gDesc_VeryLargeTriangle = "Objects on a very large triangle. Used to test accuracy limits of various contact routines.";

START_TEST(VeryLargeTriangle, CATEGORY_CONTACT_GENERATION, gDesc_VeryLargeTriangle)

	virtual	void	GetSceneParams(PINT_WORLD_CREATE& desc)
	{
		TestBase::GetSceneParams(desc);
		desc.mCamera[0] = CameraPose(Point(8.68f, 4.96f, 9.24f), Point(-0.56f, -0.25f, -0.79f));
	}

	virtual bool	CommonSetup()
	{
		CreateSingleTriangleMesh(*this, 5000.0f);
		mCreateDefaultEnvironment = false;
		return TestBase::CommonSetup();
	}

	virtual bool	Setup(Pint& pint, const PintCaps& caps)
	{
		if(!caps.mSupportRigidBodySimulation)
			return false;

		if(!CreateMeshesFromRegisteredSurfaces(pint, caps, *this))
			return false;

		if(1)
		{
			PINT_MATERIAL_CREATE MatDesc;
			MatDesc.mStaticFriction		= 1.0f;
			MatDesc.mDynamicFriction	= 1.0f;
			MatDesc.mRestitution		= 0.0f;

			const float Radius = 1.0f;
			PINT_SPHERE_CREATE SphereDesc(Radius);
			SphereDesc.mRenderer	= CreateSphereRenderer(SphereDesc.mRadius);
			SphereDesc.mMaterial	= &MatDesc;

			PintObjectHandle ShapeHandle = CreateDynamicObject(pint, &SphereDesc, Point(0.0f, Radius*2.0f, 0.0f));
			ASSERT(ShapeHandle);

			PINT_CAPSULE_CREATE CapsuleDesc(Radius, Radius);
			CapsuleDesc.mRenderer	= CreateCapsuleRenderer(CapsuleDesc.mRadius, CapsuleDesc.mHalfHeight);
			CapsuleDesc.mMaterial	= &MatDesc;

			PintObjectHandle ShapeHandle2 = CreateDynamicObject(pint, &CapsuleDesc, Point(4.0f, Radius*2.0f, 0.0f));
			ASSERT(ShapeHandle2);
		}
		return true;
	}

END_TEST(VeryLargeTriangle)

///////////////////////////////////////////////////////////////////////////////

static const char* gDesc_SphereMeshUnitTest = "Sphere-vs-mesh unit test. A repro for an old contact generation bug in PhysX.";

START_TEST(SphereMeshUnitTest, CATEGORY_CONTACT_GENERATION, gDesc_SphereMeshUnitTest)

	virtual	void	GetSceneParams(PINT_WORLD_CREATE& desc)
	{
		TestBase::GetSceneParams(desc);
		desc.mCamera[0] = CameraPose(Point(-0.01f, 0.55f, 0.99f), Point(0.09f, -0.56f, -0.82f));
	}

	virtual bool	CommonSetup()
	{
		ASSERT(!GetNbSurfaces());
		{
			const Point Offset(476.00385f, 48.477341f, 854.03748f);

			const float verts[] = {
			477.0f, 50.811596f, 853.0f, 476.0f, 50.858505f, 852.0f, 476.0f, 48.431885f, 854.0f,
			477.0f, 50.597446f, 854.0f, 477.0f, 50.811596f, 853.0f, 476.0f, 48.431885f, 854.0f,
			477.0f, 50.597446f, 854.0f, 476.0f, 48.431885f, 854.0f, 477.0f, 48.370255f, 855.0f,
			476.0f, 48.431885f, 854.0f, 474.0f, 48.624294f, 854.0f, 476.0f, 48.852272f, 856.0f,
			477.0f, 48.370255f, 855.0f, 476.0f, 48.431885f, 854.0f, 476.0f, 48.852272f, 856.0f,
			475.0f, 50.538040f, 853.0f, 476.0f, 48.431885f, 854.0f, 476.0f, 50.858505f, 852.0f,
			475.0f, 50.538040f, 853.0f, 474.0f, 48.624294f, 854.0f, 476.0f, 48.431885f, 854.0f
			};

			const udword NbFaces = 7;
			const udword NbVerts = 3*7;

			IndexedSurface* IS = CreateManagedSurface();
			bool Status = IS->Init(NbFaces, NbVerts);
			ASSERT(Status);

			Point* Verts = IS->GetVerts();
			for(udword j=0;j<NbVerts;j++)
			{
				Verts[j].x = verts[j*3+0] - Offset.x;
				Verts[j].y = verts[j*3+1] - Offset.y;
				Verts[j].z = verts[j*3+2] - Offset.z;
			}

			IndexedTriangle* F = const_cast<IndexedTriangle*>(IS->GetFaces());
			for(udword j=0;j<NbFaces;j++)
			{
				F[j].mRef[0] = j*3+0;
				F[j].mRef[1] = j*3+1;
				F[j].mRef[2] = j*3+2;
			}
		}
		mCreateDefaultEnvironment = false;
		return TestBase::CommonSetup();
	}

	virtual bool	Setup(Pint& pint, const PintCaps& caps)
	{
		if(!caps.mSupportRigidBodySimulation)
			return false;

		if(!CreateMeshesFromRegisteredSurfaces(pint, caps, *this))
			return false;

		const float Radius = 0.036977537f;

		PINT_SPHERE_CREATE SphereDesc(Radius);
		SphereDesc.mRenderer	= CreateSphereRenderer(Radius);

		PINT_OBJECT_CREATE ObjectDesc;
		ObjectDesc.mShapes		= &SphereDesc;
		ObjectDesc.mMass		= 1.0f;
		ObjectDesc.mPosition.Zero();
		CreatePintObject(pint, ObjectDesc);

		mCreateDefaultEnvironment = false;
		return true;
	}

END_TEST(SphereMeshUnitTest)

///////////////////////////////////////////////////////////////////////////////

static const char* gDesc_SphereMeshUnitTest_FC = "Sphere-vs-mesh unit test. Face contact. The sphere center initially exactly overlaps the triangle to test \
that singular contact are properly handled. This should generate a single vertical contact.";

START_TEST(SphereMeshUnitTest_FC, CATEGORY_CONTACT_GENERATION, gDesc_SphereMeshUnitTest_FC)

	virtual	void	GetSceneParams(PINT_WORLD_CREATE& desc)
	{
		TestBase::GetSceneParams(desc);
		desc.mCamera[0] = CameraPose(Point(-0.79f, 2.50f, 6.29f), Point(0.16f, -0.31f, -0.94f));
	}

	virtual bool	CommonSetup()
	{
		ASSERT(!GetNbSurfaces());
		{
			const udword NbVerts = 3;
			Point Verts[NbVerts];
			const float Scale = 4.0f;
			for(udword i=0;i<NbVerts;i++)
			{
				const float Angle = 6.28f*float(i)/float(NbVerts);
				Verts[i].x = cosf(Angle)*Scale;
				Verts[i].y = 0;
				Verts[i].z = sinf(Angle)*Scale;
			}

			const udword NbFaces = 1;

			IndexedSurface* IS = CreateManagedSurface();
			bool Status = IS->Init(NbFaces, NbVerts, Verts);
			ASSERT(Status);

			IndexedTriangle* F = const_cast<IndexedTriangle*>(IS->GetFaces());
			for(udword j=0;j<NbFaces;j++)
			{
				F[j].mRef[0] = 0;
				F[j].mRef[1] = 2;
				F[j].mRef[2] = 1;
			}
		}
		mCreateDefaultEnvironment = false;
		return TestBase::CommonSetup();
	}

	virtual bool	Setup(Pint& pint, const PintCaps& caps)
	{
		if(!caps.mSupportRigidBodySimulation)
			return false;

		if(!CreateMeshesFromRegisteredSurfaces(pint, caps, *this))
			return false;

		const float Radius = 1.0f;

		PINT_SPHERE_CREATE SphereDesc(Radius);
		SphereDesc.mRenderer	= CreateSphereRenderer(Radius);

		PINT_OBJECT_CREATE ObjectDesc;
		ObjectDesc.mShapes		= &SphereDesc;
		ObjectDesc.mMass		= 1.0f;
		ObjectDesc.mPosition.Zero();
		CreatePintObject(pint, ObjectDesc);

		mCreateDefaultEnvironment = false;
		return true;
	}

END_TEST(SphereMeshUnitTest_FC)

///////////////////////////////////////////////////////////////////////////////

static const char* gDesc_SphereMeshUnitTest_VC = "Sphere-vs-mesh unit test. Vertex contact. This should generate a single vertical contact.";

START_TEST(SphereMeshUnitTest_VC, CATEGORY_CONTACT_GENERATION, gDesc_SphereMeshUnitTest_VC)

	virtual	void	GetSceneParams(PINT_WORLD_CREATE& desc)
	{
		TestBase::GetSceneParams(desc);
		desc.mCamera[0] = CameraPose(Point(-0.79f, 2.50f, 6.29f), Point(0.16f, -0.31f, -0.94f));
	}

	virtual bool	CommonSetup()
	{
		ASSERT(!GetNbSurfaces());
		{
			const udword NbVerts = 8;
			Point Verts[NbVerts];
			Verts[0] = Point(0.0f, 0.0f, 0.0f);
			const float Scale = 4.0f;
			const float z = -4.0f;
			for(udword i=0;i<NbVerts-1;i++)
			{
				const float Angle = 6.28f*float(i)/float(NbVerts-1);
				Verts[i+1].x = cosf(Angle)*Scale;
				Verts[i+1].y = z;
				Verts[i+1].z = sinf(Angle)*Scale;
			}

			const udword NbFaces = NbVerts-1;
			udword Indices[NbFaces*3];
			for(udword i=0;i<NbFaces;i++)
			{
				Indices[i*3+0] = 0;
				Indices[i*3+2] = i+1;
				udword j = i+2;
				if(j>=NbVerts)
					j = 1;
				Indices[i*3+1] = j;
			}

			IndexedSurface* IS = CreateManagedSurface();
			bool Status = IS->Init(NbFaces, NbVerts, Verts);
			ASSERT(Status);

			IndexedTriangle* F = const_cast<IndexedTriangle*>(IS->GetFaces());
			for(udword j=0;j<NbFaces;j++)
			{
				F[j].mRef[0] = Indices[j*3+0];
				F[j].mRef[1] = Indices[j*3+1];
				F[j].mRef[2] = Indices[j*3+2];
			}
		}
		mCreateDefaultEnvironment = false;
		return TestBase::CommonSetup();
	}

	virtual bool	Setup(Pint& pint, const PintCaps& caps)
	{
		if(!caps.mSupportRigidBodySimulation)
			return false;

		if(!CreateMeshesFromRegisteredSurfaces(pint, caps, *this))
			return false;

		const float Radius = 1.0f;

		PINT_SPHERE_CREATE SphereDesc(Radius);
		SphereDesc.mRenderer	= CreateSphereRenderer(Radius);

		PINT_OBJECT_CREATE ObjectDesc;
		ObjectDesc.mShapes		= &SphereDesc;
		ObjectDesc.mMass		= 1.0f;
		ObjectDesc.mPosition	= Point(0.0f, 3.0f, 0.0f);
		CreatePintObject(pint, ObjectDesc);

		mCreateDefaultEnvironment = false;
		return true;
	}

END_TEST(SphereMeshUnitTest_VC)

///////////////////////////////////////////////////////////////////////////////

static const char* gDesc_SphereMeshUnitTest_EC = "Sphere-vs-mesh unit test. Edge contact. This should generate a single vertical contact. \
If you wait a bit, the sphere should not start moving along the edge (if it does, it means a wrong contact has been created). \
This simple test is actually quite challenging and few engines get it right.";

START_TEST(SphereMeshUnitTest_EC, CATEGORY_CONTACT_GENERATION, gDesc_SphereMeshUnitTest_EC)

	virtual	void	GetSceneParams(PINT_WORLD_CREATE& desc)
	{
		TestBase::GetSceneParams(desc);
		desc.mCamera[0] = CameraPose(Point(3.14f, 0.63f, 4.92f), Point(-0.51f, -0.17f, -0.84f));
	}

	virtual bool	CommonSetup()
	{
		ASSERT(!GetNbSurfaces());
		{
			const float Length = 2.0f;
			const float h = -2.0f;
			const float d = 2.0f;
			const udword NbVerts = 9;
			Point Verts[NbVerts];
			Verts[0] = Point(0.0f, 0.0f, -Length);
			Verts[1] = Point(0.0f, 0.0f, 0.0f);
			Verts[2] = Point(0.0f, 0.0f, Length);
			Verts[3] = Point(d, h, -Length);
			Verts[4] = Point(d, h, 0.0f);
			Verts[5] = Point(d, h, Length);
			Verts[6] = Point(-d, h, -Length);
			Verts[7] = Point(-d, h, 0.0f);
			Verts[8] = Point(-d, h, Length);

			const udword NbTris = 8;
			udword Indices[NbTris*3];
			udword o=0;
			Indices[o++] = 0;
			Indices[o++] = 1;
			Indices[o++] = 3;

			Indices[o++] = 1;
			Indices[o++] = 2;
			Indices[o++] = 4;

			IndexedSurface* IS = CreateManagedSurface();
			bool Status = IS->Init(o/3, NbVerts, Verts);
			ASSERT(Status);

			IndexedTriangle* F = const_cast<IndexedTriangle*>(IS->GetFaces());
			for(udword j=0;j<o/3;j++)
			{
				F[j].mRef[0] = Indices[j*3+0];
				F[j].mRef[1] = Indices[j*3+1];
				F[j].mRef[2] = Indices[j*3+2];
			}
		}
		mCreateDefaultEnvironment = false;
		return TestBase::CommonSetup();
	}

	virtual bool	Setup(Pint& pint, const PintCaps& caps)
	{
		if(!caps.mSupportRigidBodySimulation)
			return false;

		if(!CreateMeshesFromRegisteredSurfaces(pint, caps, *this))
			return false;

		const float Radius = 1.0f;

		PINT_SPHERE_CREATE SphereDesc(Radius);
		SphereDesc.mRenderer	= CreateSphereRenderer(Radius);

		PINT_OBJECT_CREATE ObjectDesc;
		ObjectDesc.mShapes		= &SphereDesc;
		ObjectDesc.mMass		= 1.0f;
		ObjectDesc.mPosition	= Point(0.0f, 1.0f, -0.05f);
		CreatePintObject(pint, ObjectDesc);

		mCreateDefaultEnvironment = false;
		return true;
	}

END_TEST(SphereMeshUnitTest_EC)

///////////////////////////////////////////////////////////////////////////////
